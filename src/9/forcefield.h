//
// Created by Nycshisan on 2018/8/5.
//

#ifndef ASSIGNMENTS_FORCEFIELD_H
#define ASSIGNMENTS_FORCEFIELD_H

#include "vectors.h"

class ForceField {
public:
    virtual Vec3f getAcceleration(const Vec3f &position, float mass, float t) const = 0;

    virtual ~ForceField() = default;
};

class GravityForceField : public ForceField {
    Vec3f _gravity;

public:
    explicit GravityForceField(const Vec3f &gravity) { _gravity = gravity; }

    Vec3f getAcceleration(const Vec3f &position, float mass, float t) const override { return _gravity; };
};

class ConstantForceField : public ForceField {
    Vec3f _force;

public:
    explicit ConstantForceField(const Vec3f &force) { _force = force; }

    Vec3f getAcceleration(const Vec3f &position, float mass, float t) const override { return _force * (1.f / mass); };
};

class RadialForceField : public ForceField {
    float _magnitude;

public:
    explicit RadialForceField(float magnitude) { _magnitude = magnitude; }

    Vec3f getAcceleration(const Vec3f &position, float mass, float t) const override;;
};

class VerticalForceField : public ForceField {
    float _magnitude;

public:
    explicit VerticalForceField(float magnitude) { _magnitude = magnitude; }

    Vec3f getAcceleration(const Vec3f &position, float mass, float t) const override;;
};

class WindForceField : public ForceField {
    float _magnitude;

public:
    explicit WindForceField(float magnitude) { _magnitude = magnitude; }

    Vec3f getAcceleration(const Vec3f &position, float mass, float t) const override;;
};

#endif //ASSIGNMENTS_FORCEFIELD_H
