//
// Created by Nycshisan on 2018/8/5.
//

#ifndef ASSIGNMENTS_INTEGRATOR_H
#define ASSIGNMENTS_INTEGRATOR_H

#include "particle.h"
#include "forcefield.h"

class Integrator {
public:
    virtual void Update(Particle *particle, ForceField *forcefield, float t, float dt);

    virtual ~Integrator() = default;

    virtual Vec3f getColor() = 0;
};

class EulerIntegrator : public Integrator {
public:
    void Update(Particle *particle, ForceField *forcefield, float t, float dt) override;

    Vec3f getColor() override { return Vec3f(1.f, 0.f, 0.f); }
};

class MidpointIntegrator : public Integrator {
public:
    void Update(Particle *particle, ForceField *forcefield, float t, float dt) override;

    Vec3f getColor() override { return Vec3f(0.f, 1.f, 0.f); }
};

class RungeKuttaIntegrator : public Integrator {
public:
    void Update(Particle *particle, ForceField *forcefield, float t, float dt) override;

    Vec3f getColor() override { return Vec3f(0.f, 0.f, 1.f); }
};

#endif //ASSIGNMENTS_INTEGRATOR_H
