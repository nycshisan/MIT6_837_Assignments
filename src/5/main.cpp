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

std::shared_ptr<CommandLineArgumentParser> CmdlParser;
std::shared_ptr<SceneParser> ScParser;
std::shared_ptr<RayTracer> RTracer;

void renderFunc() {
    auto imgColor = Image(CmdlParser->width, CmdlParser->height);
    auto imgNormal = Image(CmdlParser->width, CmdlParser->height);
    auto imgDepth = Image(CmdlParser->width, CmdlParser->height);

    imgColor.SetAllPixels(Vec3f(0.f, 0.f, 0.f));
    imgNormal.SetAllPixels(Vec3f(0.f, 0.f, 0.f));
    imgDepth.SetAllPixels(Vec3f(0.f, 0.f, 0.f));

    auto *camera = ScParser->getCamera();
    assert(camera != nullptr);

    for (int y = 0; y < CmdlParser->width; ++y) {
        for (int x = 0; x < CmdlParser->height; ++x) {
            Vec2f p(float(x) / CmdlParser->width, float(y) / CmdlParser->height);
            auto ray = camera->generateRay(p);
            Hit hit;
            auto color = RTracer->traceRay(ray, camera->getTMin(), 0, 1.f, 1.f, hit);

            if (CmdlParser->output_file != nullptr) {
                imgColor.SetPixel(x, y, color);
            }

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
        imgColor.SaveTGA(CmdlParser->output_file);
    }

    if (CmdlParser->depth_file != nullptr) {
        imgDepth.SaveTGA(CmdlParser->depth_file);
    }

    if (CmdlParser->normal_file != nullptr) {
        imgNormal.SaveTGA(CmdlParser->normal_file);
    }
}

void traceRayFunc(float x, float y) {
    auto *camera = ScParser->getCamera();
    assert(camera != nullptr);
    Vec2f p(x, y);
    auto ray = camera->generateRay(p);
    Hit hit;
    RTracer->traceRay(ray, camera->getTMin(), 0, 1.f, 1.f, hit);
}

int main(int argc, char* argv[]) {
    CmdlParser = std::make_shared<CommandLineArgumentParser>();
    CmdlParser->parse(argc, argv);
    ScParser = std::make_shared<SceneParser>(CmdlParser->input_file);
    RTracer = std::make_shared<RayTracer>(ScParser, CmdlParser);

    if (CmdlParser->use_gui) {
        GLCanvas canvas;
        canvas.initialize(ScParser.get(), renderFunc, traceRayFunc, nullptr, CmdlParser->visualize_grid);
    } else {
        renderFunc();
    }
}