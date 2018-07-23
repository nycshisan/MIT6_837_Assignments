//
// Created by Nycshisan on 2018/7/23.
//

#ifndef ASSIGNMENTS_SPHERE_H
#define ASSIGNMENTS_SPHERE_H

#include "object3d.h"
#include "vectors.h"

class Sphere : public Object3D {
    Vec3f _center;
    float _radius;

public:
    Sphere(const Vec3f &center, float radius, Material *material);
    bool intersect(const Ray &r, Hit &h, float tmin) override;
};

#endif //ASSIGNMENTS_SPHERE_H
