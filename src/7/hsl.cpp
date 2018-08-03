//
// Created by Nycshisan on 2018/7/31.
//

#include "hsl.h"


HSLColor::HSLColor(float h, float s, float l): _h(h), _s(s), _l(l) {}

Vec3f HSLColor::toRGBColor() {
    float r, g, b;
    if (_s == 0.0f) {
        r = g = b = _l;
    } else {
        float q = _l < 0.5f ? _l * (1 + _s) : _l + _s - _l * _s;
        float p = 2 * _l - q;
        float hk = _h / 360;
        float tr = hk + (float)1 / 3, tg = hk, tb = hk - (float)1 / 3;
        tr = _cvttf1(tr);
        tg = _cvttf1(tg);
        tb = _cvttf1(tb);
        tr = _cvttf2(tr);
        tg = _cvttf2(tg);
        tb = _cvttf2(tb);
        r = _cvttf3(tr, p, q);
        g = _cvttf3(tg, p, q);
        b = _cvttf3(tb, p, q);
    }

    return Vec3f(r, g, b);
}

float HSLColor::_cvttf1(float in) {
    if (in < 0) in += 1.0;
    return in;
}

float HSLColor::_cvttf2(float in) {
    if (in > 1) in -= 1.0;
    return in;
}

float HSLColor::_cvttf3(float in, float p, float q) {
    if (in < (float)1 / 6)
        return p + ((q - p) * 6 * in);
    else if (in < (float)1 / 2)
        return q;
    else if (in < (float)2 / 3)
        return p + ((q - p) * 6 * ((float)2 / 3 - in));
    else
        return p;
}
