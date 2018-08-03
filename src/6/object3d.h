//
// Created by Nycshisan on 2018/7/23.
//

#ifndef ASSIGNMENTS_OBJECT3D_H
#define ASSIGNMENTS_OBJECT3D_H

#include <memory>

#include <OpenGL/gl.h>

#include "vectors.h"
#include "matrix.h"

class Grid;
class Ray;
class Hit;
class Material;
class BoundingBox;

enum ObjectType {
    SphereObject, PlaneObject, TriangleObject, GridObject, GroupObject
};

class Object3D {
protected:
    Material *_m = nullptr;
    std::shared_ptr<BoundingBox> _bb;

    ObjectType _type;

public:
    virtual bool intersect(const Ray &r, Hit &h, float tmin) = 0;
    virtual void paint() = 0;

    ObjectType getObjectType() const { return _type; }
    std::shared_ptr<BoundingBox> getBoundingBox() const { return _bb; }

    virtual void insertIntoGrid(Grid *g, Matrix *m);

    virtual ~Object3D() {};
};

#endif //ASSIGNMENTS_OBJECT3D_H
