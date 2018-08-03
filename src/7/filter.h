//
// Created by Nycshisan on 2018/8/3.
//

#ifndef ASSIGNMENTS_FILTER_H
#define ASSIGNMENTS_FILTER_H

#include "vectors.h"
#include "film.h"

class Filter {
public:
    Vec3f getColor(int i, int j, Film *film);
    virtual float getWeight(float x, float y) = 0;
    virtual int getSupportRadius() = 0;
};

#endif //ASSIGNMENTS_FILTER_H
