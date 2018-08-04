//
// Created by Nycshisan on 2018/8/4.
//

#ifndef ASSIGNMENTS_SAMPLER_H
#define ASSIGNMENTS_SAMPLER_H

#include <memory>
#include <string>
#include <cmath>
#include <cassert>

#include "vectors.h"

class Sampler {
protected:
    int _nSamples = 0;

public:
    static shared_ptr<Sampler> GetSamplerByStrategy(const std::string &strategyString, int nSamples);
    static std::shared_ptr<Sampler> GetDefaultSampler();

    virtual Vec2f getSamplePosition(int n) = 0;
};

class RandomSampler : public Sampler {
public:
    Vec2f getSamplePosition(int n) override;
};

class UniformSampler : public Sampler {
protected:
    int _alignedStep;

public:
    explicit UniformSampler(int nSamples);

    Vec2f getSamplePosition(int n) override;
};

class JitteredSampler : public UniformSampler {
public:
    explicit JitteredSampler(int nSamples) : UniformSampler(nSamples) {}

    Vec2f getSamplePosition(int n) override;
};

#endif //ASSIGNMENTS_SAMPLER_H
