//
// Created by Nycshisan on 2018/7/23.
//

#ifndef ASSIGNMENTS_PLANE_H
#define ASSIGNMENTS_PLANE_H

#include "object3d.h"

class Plane : public Object3D {
    Vec3f _normal;
    float _offset;

public:
    Plane(const Vec3f &normal, float offset, Material *m);
    Plane(const Vec3f &v0, const Vec3f &v1, const Vec3f &v2, Material *m);

    bool intersect(const Ray &r, Hit &h, float tmin) override;
    void paint() override;
};

#endif //ASSIGNMENTS_PLANE_H
