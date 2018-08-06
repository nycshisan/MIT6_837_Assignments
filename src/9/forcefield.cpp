//
// Created by Nycshisan on 2018/8/5.
//

#include "forcefield.h"
#include "perlin_noise.h"

Vec3f RadialForceField::getAcceleration(const Vec3f &position, float mass, float t) const {
    auto force = position * _magnitude;
    force.Negate();
    return force * (1.f / mass);
}

Vec3f VerticalForceField::getAcceleration(const Vec3f &position, float mass, float t) const {
    Vec3f force = Vec3f(0.f, -position.y(), 0.f) * _magnitude;
    return force * (1.f / mass);
}

Vec3f WindForceField::getAcceleration(const Vec3f &position, float mass, float t) const {
    // time-based varying
    auto nx = float(PerlinNoise::noise(t, position.y(), position.z()) * 1.5);
    auto ny = float(PerlinNoise::noise(position.x(), t, position.z()) + 0.3);
    auto nz = float(PerlinNoise::noise(position.x(), position.y(), t));
    auto force = Vec3f(nx, ny, nz) * _magnitude;
    return force * (1.f / mass);
}
