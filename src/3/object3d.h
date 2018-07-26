//
// Created by Nycshisan on 2018/7/23.
//

#ifndef ASSIGNMENTS_OBJECT3D_H
#define ASSIGNMENTS_OBJECT3D_H

#include <OpenGL/gl.h>

#include "ray.h"
#include "hit.h"
#include "material.h"

class Object3D {
protected:
    Material *_m = nullptr;

public:
    virtual bool intersect(const Ray &r, Hit &h, float tmin) = 0;
    virtual void paint() = 0;
};

#endif //ASSIGNMENTS_OBJECT3D_H
