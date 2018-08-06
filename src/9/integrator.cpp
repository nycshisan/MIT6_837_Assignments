//
// Created by Nycshisan on 2018/8/5.
//

#include "integrator.h"

void Integrator::Update(Particle *particle, ForceField *forcefield, float t, float dt) {
    particle->increaseAge(dt);
}

void EulerIntegrator::Update(Particle *particle, ForceField *forcefield, float t, float dt) {
    Integrator::Update(particle, forcefield, t, dt);
    const auto &pn = particle->getPosition();
    const auto &vn = particle->getVelocity();

    auto an = forcefield->getAcceleration(pn, particle->getMass(), t);
    auto pn1 = pn + vn * dt;
    auto vn1 = vn + an * dt;

    particle->setPosition(pn1);
    particle->setVelocity(vn1);
}

void MidpointIntegrator::Update(Particle *particle, ForceField *forcefield, float t, float dt) {
    Integrator::Update(particle, forcefield, t, dt);
    const auto &pn = particle->getPosition();
    const auto &vn = particle->getVelocity();

    auto an = forcefield->getAcceleration(pn, particle->getMass(), t);
    auto pm = pn + vn * (dt / 2.f);
    auto vm = vn + an * (dt / 2.f);
    auto am = forcefield->getAcceleration(pm, particle->getMass(), t + dt / 2.f);
    auto pn1 = pn + vm * dt;
    auto vn1 = vn + am * dt;

    particle->setPosition(pn1);
    particle->setVelocity(vn1);
}

void RungeKuttaIntegrator::Update(Particle *particle, ForceField *forcefield, float t, float dt) {
    Integrator::Update(particle, forcefield, t, dt);
    const auto &pn = particle->getPosition();
    const auto &vn = particle->getVelocity();

    float mass = particle->getMass();

    auto pk1 = pn;
    auto ak1 = forcefield->getAcceleration(pk1, mass, t);
    auto vk1 = vn;

    auto pk2 = pn + vk1 * (dt / 2.f);
    auto ak2 = forcefield->getAcceleration(pk2, mass, t + dt / 2.f);
    auto vk2 = vn + ak1 * (dt / 2.f);

    auto pk3 = pn + vk2 * (dt / 2.f);
    auto ak3 = forcefield->getAcceleration(pk3, mass, t + dt / 2.f);
    auto vk3 = vn + ak2 * (dt / 2.f);

    auto pk4 = pn + vk3 * dt;
    auto ak4 = forcefield->getAcceleration(pk4, mass, t + dt);
    auto vk4 = vn + ak3 * dt;

    auto pn1 = pn + (vk1 + 2.f * vk2 + 2.f * vk3 + vk4) * (1.f / 6.f) * dt;
    auto vn1 = vn + (ak1 + 2.f * ak2 + 2.f * ak3 + ak4) * (1.f / 6.f) * dt;


    particle->setPosition(pn1);
    particle->setVelocity(vn1);
}

