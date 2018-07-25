//
// Created by Nycshisan on 2018/7/23.
//

#include "commandLineArgumentParser.h"
#include "image.h"
#include "raytracer.h"
#include "glCanvas.h"

CommandLineArgumentParser cmdlParser;
RayTracer rayTracer;

void renderFunc() {
    auto img = Image(cmdlParser.width, cmdlParser.height);
    rayTracer.renderToImage(img, cmdlParser.shade_back);
    img.SaveTGA(cmdlParser.output_file);

    if (cmdlParser.depth_file != nullptr) {
        rayTracer.renderDepthToImage(img, cmdlParser.depth_min, cmdlParser.depth_max);
        img.SaveTGA(cmdlParser.depth_file);
    }

    if (cmdlParser.normal_file != nullptr) {
        rayTracer.renderNormalToImage(img);
        img.SaveTGA(cmdlParser.normal_file);
    }
}

int main(int argc, char* argv[]) {
    cmdlParser.parse(argc, argv);

    rayTracer.readSceneFromFile(cmdlParser.input_file);

    if (cmdlParser.use_gui) {
        GLCanvas canvas;
        canvas.initialize(argc, argv, rayTracer.getSceneParser().get(), renderFunc);
    } else {
        renderFunc();
    }
}