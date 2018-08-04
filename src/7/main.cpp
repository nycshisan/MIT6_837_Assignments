//
// Created by Nycshisan on 2018/7/23.
//

#include <memory>
#include <cmath>

#include "commandLineArgumentParser.h"
#include "image.h"
#include "raytracer.h"
#include "scene_parser.h"
#include "glCanvas.h"
#include "camera.h"
#include "rayTree.h"
#include "film.h"
#include "sampler.h"
#include "filter.h"
#include "raytracing_stats.h"

std::shared_ptr<CommandLineArgumentParser> CmdlParser;
std::shared_ptr<SceneParser> ScParser;
std::shared_ptr<RayTracer> RTracer;

void traceRayFunc(float x, float y) {
    auto *camera = ScParser->getCamera();
    assert(camera != nullptr);
    Vec2f p(x, y);
    auto ray = camera->generateRay(p);
    Hit hit;
    RTracer->getGrid()->refreshColorSchema();
    RTracer->traceRay(ray, camera->getTMin(), 0, 1.f, 1.f, hit);
}

void renderFunc() {
    RayTracingStats::Initialize(CmdlParser->width, CmdlParser->height, RTracer->getBoundingBox(),
                                CmdlParser->grid_nx, CmdlParser->grid_ny, CmdlParser->grid_nz);

    auto imgColor = Image(CmdlParser->width, CmdlParser->height);
    auto imgNormal = Image(CmdlParser->width, CmdlParser->height);
    auto imgDepth = Image(CmdlParser->width, CmdlParser->height);

    imgColor.SetAllPixels(Vec3f(0.f, 0.f, 0.f));
    imgNormal.SetAllPixels(Vec3f(0.f, 0.f, 0.f));
    imgDepth.SetAllPixels(Vec3f(0.f, 0.f, 0.f));

    auto *camera = ScParser->getCamera();
    assert(camera != nullptr);

    std::shared_ptr<Film> film;
    std::shared_ptr<Sampler> sampler;
    std::shared_ptr<Filter> filter;

    if (CmdlParser->sampler_strategy != nullptr) {
        film = std::make_shared<Film>(CmdlParser->width, CmdlParser->height, CmdlParser->samples_num);
        sampler = Sampler::GetSamplerByStrategy(CmdlParser->sampler_strategy, CmdlParser->samples_num);
    } else {
        film = std::make_shared<Film>(CmdlParser->width, CmdlParser->height, 1);
        sampler = Sampler::GetDefaultSampler();
    }

    if (CmdlParser->filter_strategy != nullptr) {
        filter = Filter::GetFilterByStrategy(CmdlParser->filter_strategy, CmdlParser->filter_argument);
    } else {
        filter = Filter::GetDefaultFilter();
    }

    for (int x = 0; x < CmdlParser->width; ++x) {
        for (int y = 0; y < CmdlParser->height; ++y) {
            float maxImageSize = std::max(CmdlParser->width, CmdlParser->height);
            float xMargin = (maxImageSize - CmdlParser->width) / 2.f;
            float yMargin = (maxImageSize - CmdlParser->height) / 2.f;
            Vec2f pixelPos((float(x) + xMargin) / maxImageSize, (float(y) + yMargin) / maxImageSize);
            Vec2f pixelSize(1.f / CmdlParser->width, 1.f / CmdlParser->height);

            Hit hit;

            if (x == 0 && y == 1) {

            }

            for (int i = 0; i < film->getNumSamples(); ++i) {
                Vec2f samplePos = sampler->getSamplePosition(i);
                auto ray = camera->generateRay(pixelPos + samplePos * pixelSize);
                auto color = RTracer->traceRay(ray, camera->getTMin(), 0, 1.f, 1.f, hit);
                film->setSample(x, y, i, samplePos, color);
            }
            // now we do not do super sampling for depth and normal
            if (CmdlParser->depth_file != nullptr) {
                auto t = hit.getT();
                t = std::min(CmdlParser->depth_max, t);
                t = std::max(CmdlParser->depth_min, t);
                float weight = (t - CmdlParser->depth_min) / (CmdlParser->depth_max - CmdlParser->depth_min);
                Vec3f depthColor;
                Vec3f::WeightedSum(depthColor, Vec3f(0.f, 0.f, 0.f), weight, Vec3f(1.f, 1.f, 1.f), 1 - weight);
                imgDepth.SetPixel(x, y, depthColor);
            }
            if (CmdlParser->normal_file != nullptr) {
                auto norm = hit.getNormal();
                Vec3f normalColor = Vec3f(std::fabsf(norm.x()), std::fabsf(norm.y()), std::fabsf(norm.z()));
                imgNormal.SetPixel(x, y, normalColor);
            }
        }
    }

    if (CmdlParser->output_file != nullptr) {
        for (int x = 0; x < CmdlParser->width; ++x) {
            for (int y = 0; y < CmdlParser->height; ++y) {
                imgColor.SetPixel(x, y, filter->getColor(x, y, film.get()));
            }
        }
        imgColor.SaveTGA(CmdlParser->output_file);
    }
    if (CmdlParser->depth_file != nullptr) {
        imgDepth.SaveTGA(CmdlParser->depth_file);
    }
    if (CmdlParser->normal_file != nullptr) {
        imgNormal.SaveTGA(CmdlParser->normal_file);
    }
    if (CmdlParser->samples_file != nullptr) {
        film->renderSamples(CmdlParser->samples_file, CmdlParser->samples_zoom);
    }
    if (CmdlParser->filter_file != nullptr) {
        film->renderFilter(CmdlParser->filter_file, CmdlParser->filter_zoom, filter.get());
    }

    if (CmdlParser->stats)
        RayTracingStats::PrintStatistics();
}

int main(int argc, char* argv[]) {
    srand48(time(nullptr));
    CmdlParser = std::make_shared<CommandLineArgumentParser>();
    CmdlParser->parse(argc, argv);
    ScParser = std::make_shared<SceneParser>(CmdlParser->input_file);
    RTracer = std::make_shared<RayTracer>(ScParser, CmdlParser);

    if (CmdlParser->use_gui) {
        GLCanvas canvas;
        canvas.initialize(ScParser.get(), renderFunc, traceRayFunc, RTracer->getGrid(), CmdlParser->visualize_grid);
    } else {
        renderFunc();
    }
}