//
// Created by Nycshisan on 2018/7/23.
//

#include "triangle.h"

static float _err = 1e-3f;

Triangle::Triangle(const Vec3f &v0, const Vec3f &v1, const Vec3f &v2, Material *m) : _plane(v0, v1, v2, m) {
    _v0 = v0;
    _v1 = v1;
    _v2 = v2;
    _m = m;

    _barycenter = (v0 + v1 + v2) * (1.f / 3.f);

    Vec3f v0v1 = v1 - v0, v1v2 = v2 - v1;
    Vec3f::Cross3(_norm, v0v1, v1v2);
    _norm.Normalize();
}

bool Triangle::intersect(const Ray &r, Hit &h, float tmin) {
    Vec3f c, edge, pv, intersection;
    Hit hit;
    if (_plane.intersect(r, hit, tmin)) {
        if (hit.getT() >= tmin) {
            // test inside the tri
            intersection = hit.getIntersectionPoint();
            edge = _v0 - _v1;
            pv = intersection - _v0;
            Vec3f::Cross3(c, pv, edge);
            if (c.Dot3(_norm) < -_err)
                return false;
            edge = _v1 - _v2;
            pv = intersection - _v1;
            Vec3f::Cross3(c, pv, edge);
            if (c.Dot3(_norm) < -_err)
                return false;
            edge = _v2 - _v0;
            pv = intersection - _v2;
            Vec3f::Cross3(c, pv, edge);
            if (c.Dot3(_norm) < -_err)
                return false;
            h = hit;
            return true;
        } else {
            return false;
        }
    } else {
        return false;
    }
}

void Triangle::paint() {
    GLfloat x, y, z;

    _m->glSetMaterial();

    glBegin(GL_TRIANGLES);
    _norm.Get(x, y, z);
    glNormal3f(x, y, z);
    _v0.Get(x, y, z);
    glVertex3f(x, y, z);
    _v1.Get(x, y, z);
    glVertex3f(x, y, z);
    _v2.Get(x, y, z);
    glVertex3f(x, y, z);
    glEnd();
}
