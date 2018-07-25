//
// Created by Nycshisan on 2018/7/23.
//

#include "sphere.h"

#include <cmath>

bool Sphere::intersect(const Ray &r, Hit &h, float tmin) {
    const auto &origin = r.getOrigin();
    auto dOrigin = origin - _center;
    auto squareLenOrigin = dOrigin.Dot3(dOrigin);
    auto squareRadius = _radius * _radius;
    auto dFootPointToOrigin = -dOrigin.Dot3(r.getDirection());
    auto squareDistRay = squareLenOrigin - dFootPointToOrigin * dFootPointToOrigin;
    if (squareDistRay > squareRadius) {
        // no hit
        return false;
    }
    auto squareDistIntersectionToFootPoint = squareRadius - squareDistRay;
    auto dIntersectionToOrigin_1 = dFootPointToOrigin + sqrt(squareDistIntersectionToFootPoint);
    auto dIntersectionToOrigin_2 = dFootPointToOrigin - sqrt(squareDistIntersectionToFootPoint);
    auto dIntersectionToOriginMin = std::min(dIntersectionToOrigin_1, dIntersectionToOrigin_2);
    auto dIntersectionToOriginMax = std::max(dIntersectionToOrigin_1, dIntersectionToOrigin_2);
    float t;
    if (dIntersectionToOriginMin >= tmin) {
        t = dIntersectionToOriginMin;
    } else if (dIntersectionToOriginMax >= tmin) {
        t = dIntersectionToOriginMax;
    } else {
        return false;
    }
    h = Hit(t, _m);
    return true;
}

Sphere::Sphere(const Vec3f &center, float radius, Material *material) {
    _center = center;
    _radius = radius;
    _m = material;
}
