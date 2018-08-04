//
// Created by Nycshisan on 2018/8/3.
//

#include "filter.h"

static float _err = 1e-5f;

std::shared_ptr<Filter> Filter::GetFilterByStrategy(const std::string &strategyString, float filterArgument) {
    if (strategyString == "-box_filter") {
        return std::make_shared<BoxFilter>(filterArgument);
    } else if (strategyString == "-tent_filter") {
        return std::make_shared<TentFilter>(filterArgument);
    } else if (strategyString == "-gaussian_filter") {
        return std::make_shared<GaussianFilter>(filterArgument);
    }
    assert(0);
    return nullptr;
}

std::shared_ptr<Filter> Filter::GetDefaultFilter() {
    return std::make_shared<BoxFilter>(_err);
}

Vec3f Filter::getColor(int i, int j, Film *film) {
    Vec3f color;

    float sumWeight = 0.f;

    int supportRadius = getSupportRadius();
    for (int xOffset = -supportRadius; xOffset <= supportRadius; ++xOffset) {
        for (int yOffset = -supportRadius; yOffset <= supportRadius; ++yOffset) {
            int x = xOffset + i, y = yOffset + j;
            if (x < 0 || x >= film->getWidth() || y < 0 || y >= film->getHeight())
                continue; // out of image, discard
            for (int n = 0; n < film->getNumSamples(); ++n) {
                auto sample = film->getSample(x, y, n);
                auto samplePos = sample.getPosition();
                auto pixelPos = Vec2f(samplePos.x() + xOffset - 0.5f, samplePos.y() + yOffset - 0.5f);
                auto weight = getWeight(pixelPos.x(), pixelPos.y());
                sumWeight += weight;
                color += sample.getColor() * weight;
            }
        }
    }
    float normalizeFactor = 1.f / sumWeight;
    return color * normalizeFactor;
}

float BoxFilter::getWeight(float x, float y) {
    return fabsf(x) < _radius && fabsf(y) < _radius;
}

int BoxFilter::getSupportRadius() {
    return int(ceilf(_radius - 0.5f));
}

float TentFilter::getWeight(float x, float y) {
    float dist = sqrtf(x * x + y * y);
    return std::max(0.f, 1.f - dist / _radius);
}

int TentFilter::getSupportRadius() {
    return int(ceilf(_radius - 0.5f));
}

float GaussianFilter::getWeight(float x, float y) {
    float dist = sqrtf(x * x + y * y);
    return expf((-dist * dist) / (2.f * _sigma * _sigma));
}

int GaussianFilter::getSupportRadius() {
    return int(ceilf(2 * _sigma - 0.5f));
}
