//
// Created by Nycshisan on 2018/7/23.
//

#ifndef ASSIGNMENTS_GROUP_H
#define ASSIGNMENTS_GROUP_H

#include <vector>

#include "object3d.h"

class Group : public Object3D {
    unsigned long _nObjects;
    std::vector<Object3D*> _objs;

public:
    explicit Group (int nObjects);

    bool intersect(const Ray &r, Hit &h, float tmin) override;
    void addObject(int index, Object3D *obj);
};

#endif //ASSIGNMENTS_GROUP_H
