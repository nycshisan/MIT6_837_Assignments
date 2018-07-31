//
// Created by Nycshisan on 2018/7/23.
//

#include "group.h"

#include <limits>

Group::Group(int nObjects) {
    assert(nObjects >= 0);
    _nObjects = (unsigned long)nObjects;
    _objs.resize(_nObjects);

    float maxf = std::numeric_limits<float>::max(), minf = std::numeric_limits<float>::lowest();
    _boundingBox = std::make_shared<BoundingBox>(Vec3f(maxf, maxf, maxf), Vec3f(minf, minf, minf));
}

bool Group::intersect(const Ray &r, Hit &h, float tmin) {
    bool success = false;
    for (auto &obj: _objs) {
        Hit currentHit;
        if (obj->intersect(r, currentHit, tmin)) {
            success = true;
            if (currentHit.getT() < h.getT()) {
                h = currentHit;
            }
        }
    }
    return success;
}

void Group::addObject(int index, Object3D *obj) {
    assert(index < _nObjects);
    _objs[index] = obj;
    if (obj->getBoundingBox()) // discard those infinite bounding boxes such as planes'
        _boundingBox->Extend(obj->getBoundingBox().get());
}

void Group::paint() {
    for (int i = 0; i < _nObjects; ++i) {
        _objs[i]->paint();
    }
}

void Group::insertIntoGrid(Grid *g, Matrix *m) {
    for (auto &obj: _objs) {
        obj->insertIntoGrid(g, m);
    }
}
