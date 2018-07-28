//
// Created by Nycshisan on 2018/7/23.
//

#include "plane.h"

#include <cmath>

static float _err = 1e-3f;

Plane::Plane(const Vec3f &normal, float offset, Material *m) {
    _normal = normal;
    _offset = offset;
    _m = m;
}

bool Plane::intersect(const Ray &r, Hit &h, float tmin) {
    Hit hit;
    auto nRd = _normal.Dot3(r.getDirection());
    if (std::fabsf(nRd) < _err) {
        // vertical
        return false;
    }
    float t = (_offset - _normal.Dot3(r.getOrigin())) / nRd;
    hit.set(t, _m, _normal, r, Hit::ObjectType::Plane);
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

void Plane::paint() {
    _m->glSetMaterial();

    Vec3f v;
    if (fabsf(_normal.y()) < 1e-3 && fabsf(_normal.z()) < 1e-3) {
        v.Set(0.f, 1.f, 0.f);
    } else {
        v.Set(1.f, 0.f, 0.f);
    }
    Vec3f b1, b2, b3, b4;
    Vec3f::Cross3(b1, v, _normal);
    Vec3f::Cross3(b2, _normal, b1);

    float big = 1e4;

    b1.Normalize();
    b3 = b1;
    b3.Negate();
    b1 *= big;
    b1 += _normal * _offset;
    b3 *= big;
    b3 += _normal * _offset;

    b2.Normalize();
    b4 = b2;
    b4.Negate();
    b2 *= big;
    b2 += _normal * _offset;
    b4 *= big;
    b4 += _normal * _offset;

    GLfloat x, y, z;

    glBegin(GL_QUADS);
    _normal.Get(x, y, z);
    glNormal3f(x, y, z);
    b1.Get(x, y, z);
    glVertex3f(x, y, z);
    b2.Get(x, y, z);
    glVertex3f(x, y, z);
    b3.Get(x, y, z);
    glVertex3f(x, y, z);
    b4.Get(x, y, z);
    glVertex3f(x, y, z);
    glEnd();
}
