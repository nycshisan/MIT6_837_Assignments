//
// Created by Nycshisan on 2018/7/23.
//

#include "group.h"

#include "limits"

bool Group::intersect(const Ray &r, Hit &h, float tmin) {
    h = Hit(std::numeric_limits<float>::max(), nullptr);
    bool success = false;
    for (auto obj: _objs) {
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
}

Group::Group(int nObjects) {
    assert(nObjects >= 0);
    _nObjects = (unsigned long)nObjects;
    _objs.resize(_nObjects);
}
