//
// Created by Nycshisan on 2018/7/23.
//

#ifndef ASSIGNMENTS_RAYTRACER_H
#define ASSIGNMENTS_RAYTRACER_H

#include <memory>

#include "scene_parser.h"
#include "image.h"
#include "camera.h"
#include "group.h"
#include "hit.h"
#include "material.h"

class RayTracer {
    std::shared_ptr<SceneParser> _sceneParser;

public:
    void readSceneFromFile(char *input_file);
    void renderToImage(Image &img, bool shade_back = false);
    void renderDepthToImage(Image &img, float depMin, float depMax);
    void renderNormalToImage(Image &img);
};

#endif //ASSIGNMENTS_RAYTRACER_H
