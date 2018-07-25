//
// Created by Nycshisan on 2018/7/23.
//

#include "plane.h"

#include <cmath>

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

void Plane::paint() {
    _m->glSetMaterial();

    Vec3f v;
    if (fabsf(_normal.y()) < 1e-3 && fabsf(_normal.z()) < 1e-3) {
        v.Set(0.f, 1.f, 0.f);
    } else {
        v.Set(1.f, 0.f, 0.f);
    }
    Vec3f b1, b2;
    Vec3f::Cross3(b1, v, _normal);
    Vec3f::Cross3(b2, _normal, b1);

    b1.Normalize();
    b1 *= float(1e4);
    b2.Normalize();
    b2 *= float(1e4);

    GLfloat x, y, z;

    glBegin(GL_QUADS);
    _normal.Get(x, y, z);
    glNormal3f(x, y, z);
    b1.Get(x, y, z);
    glVertex3f(x, y, z);
    b2.Get(x, y, z);
    glVertex3f(x, y, z);
    b1.Negate();
    b1.Get(x, y, z);
    glVertex3f(x, y, z);
    b2.Negate();
    b2.Get(x, y, z);
    glVertex3f(x, y, z);
    glEnd();
}
