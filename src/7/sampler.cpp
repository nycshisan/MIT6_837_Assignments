//
// Created by Nycshisan on 2018/8/4.
//

#include "sampler.h"

shared_ptr<Sampler> Sampler::GetSamplerByStrategy(const std::string &strategyString, int nSamples) {
    if (strategyString == "-random_samples") {
        return std::make_shared<RandomSampler>();
    } else if (strategyString == "-uniform_samples") {
        return std::make_shared<UniformSampler>(nSamples);
    } else if (strategyString == "-jittered_samples") {
        return std::make_shared<JitteredSampler>(nSamples);
    }
    assert(0);
    return nullptr;
}

std::shared_ptr<Sampler> Sampler::GetDefaultSampler() {
    return std::make_shared<UniformSampler>(1);
}

Vec2f RandomSampler::getSamplePosition(int n) {
    return Vec2f(static_cast<float>(drand48()), static_cast<float>(drand48()));
}

UniformSampler::UniformSampler(int nSamples) {
    assert(int(sqrtf(nSamples)) * int(sqrtf(nSamples)) == int(roundf(nSamples)));
    _nSamples = nSamples;
    _alignedStep = int(sqrtf(_nSamples));
}

Vec2f UniformSampler::getSamplePosition(int n) {
    int x = n % _alignedStep, y = n / _alignedStep;
    float xPos = 1.f / _alignedStep * (x + 0.5f), yPos = 1.f / _alignedStep * (y + 0.5f);
    return Vec2f(xPos, yPos);
}

Vec2f JitteredSampler::getSamplePosition(int n) {
    auto uniformPos = UniformSampler::getSamplePosition(n);
    auto turbulence = Vec2f(static_cast<float>(drand48()) - 0.5f, static_cast<float>(drand48()) - 0.5f) / float(_alignedStep);
    auto jitteredPos = uniformPos + turbulence;
    jitteredPos.Clamp();
    return jitteredPos;
}
