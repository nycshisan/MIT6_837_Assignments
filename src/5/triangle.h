//
// Created by Nycshisan on 2018/7/23.
//

#ifndef ASSIGNMENTS_TRIANGLE_H
#define ASSIGNMENTS_TRIANGLE_H

#include "object3d.h"
#include "plane.h"
#include "material.h"
#include "vectors.h"

class Triangle : public Object3D {
    Vec3f _v0, _v1, _v2, _norm;
    Plane _plane;

public:
    Triangle(const Vec3f &v0, const Vec3f &v1, const Vec3f &v2, Material *m);

    bool intersect(const Ray &r, Hit &h, float tmin) override;
    void paint() override;

    void insertIntoGrid(Grid *g, Matrix *m) override;

    Vec3f getVertex(int index) const {
        switch (index) {
            case 0: return _v0;
            case 1: return _v1;
            case 2: return _v2;
            default: assert(0); return Vec3f();
        }
    }
};

#endif //ASSIGNMENTS_TRIANGLE_H
