//
// Created by Nycshisan on 2018/7/23.
//

#include "plane.h"

Plane::Plane(const Vec3f &normal, float offset, Material *m) {
    _normal = normal;
    _offset = offset;
    _m = m;
}

bool Plane::intersect(const Ray &r, Hit &h, float tmin) {
    Hit hit;
    float t = (_offset - _normal.Dot3(r.getOrigin())) / _normal.Dot3(r.getDirection());
    hit.set(t, _m, _normal, r);
    if (t >= tmin) {
        h = hit;
        return true;
    }
    return false;
}

Plane::Plane(const Vec3f &v0, const Vec3f &v1, const Vec3f &v2, Material *m) {
    Vec3f::Cross3(_normal, v1 - v0, v2 - v1);
    _normal.Normalize();
    _offset = _normal.Dot3(v0);
    _m = m;
}
