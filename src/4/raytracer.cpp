//
// Created by Nycshisan on 2018/7/23.
//

#include "raytracer.h"

#include <vector>

#include "group.h"
#include "light.h"

RayTracer::RayTracer(const std::shared_ptr<SceneParser> &scParser, const shared_ptr<CommandLineArgumentParser> &cmdlParser) {
    _sceneParser = scParser;
    _cmdlParser = cmdlParser;
}

Vec3f RayTracer::traceRay(Ray &ray, float tmin, int bounces, float weight, float indexOfRefraction, Hit &hit) const {
    auto color = _sceneParser->getBackgroundColor();

    auto group = _sceneParser->getGroup();

    auto nLights = _sceneParser->getNumLights();
    std::vector<Light*> lights(static_cast<unsigned long>(nLights));
    for (int i = 0; i < nLights; ++i) {
        lights[i] = _sceneParser->getLight(i);
    }
    auto ambient = _sceneParser->getAmbientLight();

    if (group->intersect(ray, hit, tmin)) {
        auto norm = hit.getNormal();
        if (_cmdlParser->shade_back && norm.Dot3(ray.getDirection()) > 0) {
            norm.Negate();
        }

        auto m = hit.getMaterial();
        auto cObj = m->getDiffuseColor();
        color = ambient * cObj;
        for (int i = 0; i < nLights; ++i) {
            Vec3f intersection = hit.getIntersectionPoint(), dirToLight, cLight;
            float distToLight;
            lights[i]->getIllumination(intersection, dirToLight, cLight, distToLight);
            color += std::max(0.f, dirToLight.Dot3(norm)) * cLight * cObj;
            color += m->Shade(ray, hit, dirToLight, cLight);
        }
    }

    return color;
}
