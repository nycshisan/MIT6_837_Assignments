//
// Created by Nycshisan on 2018/8/5.
//

#include "generator.h"

#include <OpenGL/gl.h>

float Generator::_getRandomComponent(float randomness) {
    return randomness * float(_rg.next() - 0.5) * 2.f;
}

int Generator::numNewParticles(float current_time, float dt) const {
    return int(ceilf(dt * _desiredNumParticles / _lifespan));
}

Particle *HoseGenerator::Generate(float current_time, int i) {
    auto p = _position + _positionRandomness * _rg.randomVector();
    auto v = _velocity + _velocityRandomness * _rg.randomVector();

    auto *particle = new Particle(p, v, _color, _deadColor, _mass, _lifespan);
    return particle;
}

int RingGenerator::numNewParticles(float current_time, float dt) const {
    return int(ceilf(dt * _desiredNumParticles / _lifespan * (_radiusVelocity * current_time)));
}

Particle *RingGenerator::Generate(float current_time, int i) {
    float radius = current_time * _radiusVelocity * (1.f + _radiusRandomness * _rg.next());
    float randomRadian = _rg.next() * 2.f * float(M_PI);
    float sinP = sinf(randomRadian), cosP = cosf(randomRadian);
    auto p = Vec3f(cosP * radius, 0.f, sinP * radius);
    auto v = _velocity + _velocityRandomness * _rg.randomVector();

    auto *particle = new Particle(p, v, _color, _deadColor, _mass, _lifespan);
    return particle;
}

void RingGenerator::Paint() const {
    // draw the xOz plane
    glColor3f(0.3f, 0.3f, 0.3f);
//    glNormal3f(0.f, 1.f, 0.f);
    glBegin(GL_QUADS);
    glVertex3f(10.f, 0.f, 10.f);
    glVertex3f(-10.f, 0.f, 10.f);
    glVertex3f(-10.f, 0.f, -10.f);
    glVertex3f(10.f, 0.f, -10.f);
    glEnd();
}
