//
// Created by Nycshisan on 2018/8/5.
//

#ifndef ASSIGNMENTS_INTEGRATOR_H
#define ASSIGNMENTS_INTEGRATOR_H

#include "particle.h"
#include "forcefield.h"

class Integrator {
public:
    virtual void Update(Particle *particle, ForceField *forcefield, float t, float dt) = 0;

    virtual ~Integrator() = default;

    Vec3f getColor() { assert(0);  return Vec3f(); }
};

class EulerIntegrator : public Integrator {
public:
    void Update(Particle *particle, ForceField *forcefield, float t, float dt) override { assert(0); }
};

class MidpointIntegrator : public Integrator {
public:
    void Update(Particle *particle, ForceField *forcefield, float t, float dt) override { assert(0); }
};

class RungeKuttaIntegrator : public Integrator {
public:
    void Update(Particle *particle, ForceField *forcefield, float t, float dt) override { assert(0); }
};

#endif //ASSIGNMENTS_INTEGRATOR_H
