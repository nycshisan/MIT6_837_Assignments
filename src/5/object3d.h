//
// Created by Nycshisan on 2018/7/23.
//

#ifndef ASSIGNMENTS_OBJECT3D_H
#define ASSIGNMENTS_OBJECT3D_H

#include <memory>

#include <OpenGL/gl.h>

#include "ray.h"
#include "hit.h"
#include "material.h"
#include "boundingbox.h"

class Grid;

class Object3D {
protected:
    Material *_m = nullptr;
    std::shared_ptr<BoundingBox> _boundingBox;

public:
    virtual bool intersect(const Ray &r, Hit &h, float tmin) = 0;
    virtual void paint() = 0;

    std::shared_ptr<BoundingBox> getBoundingBox() { return _boundingBox; }
    virtual void insertIntoGrid(Grid *g, Matrix *m);
};

#endif //ASSIGNMENTS_OBJECT3D_H
