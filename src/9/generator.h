//
// Created by Nycshisan on 2018/8/5.
//

#ifndef ASSIGNMENTS_GENERATOR_H
#define ASSIGNMENTS_GENERATOR_H

#include "vectors.h"
#include "particle.h"

class Generator {
public:
    // initialization
    void SetColors(Vec3f color, Vec3f dead_color, float color_randomness) { assert(0); }
    void SetLifespan(float lifespan, float lifespan_randomness, int desired_num_particles) { assert(0); }
    void SetMass(float mass, float mass_randomness) { assert(0); }

    // on each timestep, create some particles
    int numNewParticles(float current_time, float dt) const { assert(0);  return 0; }
    Particle* Generate(float current_time, int i) { assert(0);  return nullptr; }

    // for the gui
    void Paint() const { assert(0); }
    void Restart() { assert(0); }
};

class HoseGenerator : public Generator {
public:
    HoseGenerator(Vec3f position, float position_randomness, Vec3f velocity, float velocity_randomness) { assert(0); }
};

class RingGenerator : public Generator {
public:
    RingGenerator(float position_randomness, Vec3f velocity, float velocity_randomness) { assert(0); }
};

#endif //ASSIGNMENTS_GENERATOR_H
