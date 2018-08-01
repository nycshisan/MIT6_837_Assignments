//
// Created by Nycshisan on 2018/7/23.
//

#include "triangle.h"

#include "grid.h"
#include "boundingbox.h"

static float _err = 1e-3f;

Triangle::Triangle(const Vec3f &v0, const Vec3f &v1, const Vec3f &v2, Material *m) : _plane(v0, v1, v2, m) {
    _type = ObjectType::TriangleObject;

    _v0 = v0;
    _v1 = v1;
    _v2 = v2;
    _m = m;

    Vec3f v0v1 = v1 - v0, v1v2 = v2 - v1;
    Vec3f::Cross3(_norm, v0v1, v1v2);
    _norm.Normalize();

    float maxf = std::numeric_limits<float>::max(), minf = std::numeric_limits<float>::lowest();
    _bb = std::make_shared<BoundingBox>(Vec3f(maxf, maxf, maxf), Vec3f(minf, minf, minf));
    _bb->Extend(v0);
    _bb->Extend(v1);
    _bb->Extend(v2);
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

void Triangle::insertIntoGrid(class Grid *g, Matrix *m) {
    if (m != nullptr) {
        Object3D::insertIntoGrid(g, m);
        return;
    }

    int iMin, jMin, kMin, iMax, jMax, kMax;
    g->getGridCellIndex(_bb->getMin(), iMin, jMin, kMin);
    g->getGridCellIndex(_bb->getMax(), iMax, jMax, kMax);
    for (int i = iMin; i <= iMax; ++i) {
        for (int j = jMin; j <= jMax; ++j) {
            for (int k = kMin; k <= kMax; ++k) {
                g->cells[i][j][k].emplace_back(this);
            }
        }
    }
}