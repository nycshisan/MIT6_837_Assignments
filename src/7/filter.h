//
// Created by Nycshisan on 2018/8/3.
//

#ifndef ASSIGNMENTS_FILTER_H
#define ASSIGNMENTS_FILTER_H

#include <string>

#include "vectors.h"
#include "film.h"

class Filter {
public:
    static std::shared_ptr<Filter> GetFilterByStrategy(const std::string &strategyString, float filterArgument);
    static std::shared_ptr<Filter> GetDefaultFilter();

    Vec3f getColor(int i, int j, Film *film);

    virtual float getWeight(float x, float y) = 0;
    virtual int getSupportRadius() = 0;
};

class BoxFilter : public Filter {
    float _radius;
public:
    explicit BoxFilter(float radius) : _radius(radius) {}

    float getWeight(float x, float y) override;
    int getSupportRadius() override;
};

class TentFilter : public Filter {
    float _radius;
public:
    explicit TentFilter(float radius) : _radius(radius) {}

    float getWeight(float x, float y) override;
    int getSupportRadius() override;
};

class GaussianFilter : public Filter {
    float _sigma;

public:
    explicit GaussianFilter(float sigma) : _sigma(sigma) {}

    float getWeight(float x, float y) override;
    int getSupportRadius() override;
};

#endif //ASSIGNMENTS_FILTER_H
