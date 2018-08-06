//
// Created by Nycshisan on 2018/8/5.
//

#ifndef ASSIGNMENTS_GENERATOR_H
#define ASSIGNMENTS_GENERATOR_H

#include "vectors.h"
#include "particle.h"
#include "random.h"

class Generator {
protected:
    Vec3f _color, _deadColor;
    float _colorRandomness = 0.f;

    float _lifespan = 0.f, _lifespanRandomness = 0.f;
    int _desiredNumParticles = 0;

    float _mass = 0.f, _massRandomness = 0.f;

    Random _rg;

public:
    // initialization
    void SetColors(const Vec3f &color, const Vec3f &dead_color, float color_randomness) {
        _color = color;
        _deadColor = dead_color;
        _colorRandomness = color_randomness;
    }
    void SetLifespan(float lifespan, float lifespan_randomness, int desired_num_particles) {
        _lifespan = lifespan;
        _lifespanRandomness = lifespan_randomness;
        _desiredNumParticles = desired_num_particles;
    }
    void SetMass(float mass, float mass_randomness) {
        _mass = mass;
        _massRandomness = mass_randomness;
    }

    // on each timestep, create some particles
    virtual int numNewParticles(float current_time, float dt) const;
    virtual Particle* Generate(float current_time, int i) = 0;

    // for the gui
    virtual void Paint() const {}
    void Restart() { _rg = Random(); }

    virtual ~Generator() = default;
};

class HoseGenerator : public Generator {
    Vec3f _position, _velocity;
    float _positionRandomness, _velocityRandomness;

public:
    HoseGenerator(const Vec3f &position, float position_randomness, const Vec3f &velocity, float velocity_randomness) {
        _position = position; _positionRandomness = position_randomness;
        _velocity = velocity; _velocityRandomness = velocity_randomness;
    }

    Particle* Generate(float current_time, int i) override;
};

class RingGenerator : public Generator {
    Vec3f _velocity;
    float _positionRandomness, _velocityRandomness, _radiusVelocity = 0.1f, _radiusRandomness = 0.2f;

public:
    RingGenerator(float position_randomness, const Vec3f &velocity, float velocity_randomness) {
        _velocity = velocity;
        _positionRandomness = position_randomness;
        _velocityRandomness = velocity_randomness;
    }

    int numNewParticles(float current_time, float dt) const override;
    Particle* Generate(float current_time, int i) override;

    void Paint() const override;
};

#endif //ASSIGNMENTS_GENERATOR_H
