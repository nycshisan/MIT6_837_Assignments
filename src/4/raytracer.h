//
// Created by Nycshisan on 2018/7/23.
//

#ifndef ASSIGNMENTS_RAYTRACER_H
#define ASSIGNMENTS_RAYTRACER_H

#include <memory>

#include "scene_parser.h"
#include "commandLineArgumentParser.h"
#include "ray.h"
#include "hit.h"

class RayTracer {
    std::shared_ptr<SceneParser> _sceneParser;
    std::shared_ptr<CommandLineArgumentParser> _cmdlParser;

public:
    RayTracer(const std::shared_ptr<SceneParser> &scParser, const shared_ptr<CommandLineArgumentParser> &cmdlParser);

    Vec3f traceRay(Ray &ray, float tmin, int bounces, float weight, float indexOfRefraction, Hit &hit) const;
};

#endif //ASSIGNMENTS_RAYTRACER_H
