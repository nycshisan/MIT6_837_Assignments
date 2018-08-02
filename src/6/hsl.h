//
// Created by Nycshisan on 2018/7/31.
//

#ifndef ASSIGNMENTS_HSL_H
#define ASSIGNMENTS_HSL_H

#include "vectors.h"

struct HSLColor {
    float _h, _s, _l;

    float _cvttf1(float in);
    float _cvttf2(float in);
    float _cvttf3(float in, float p, float q);

public:
    HSLColor(float h, float s, float l);

    Vec3f toRGBColor();
};

#endif //ASSIGNMENTS_HSL_H
