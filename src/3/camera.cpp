//
// Created by Nycshisan on 2018/7/23.
//

#include "camera.h"

#include <limits>

Ray OrthographicCamera::generateRay(Vec2f point) {
    auto origin = _center + (point.x() - 0.5) * _size * _horizontal + (point.y() - 0.5) * _size * _up;
    return Ray(origin, _direction);
}

float OrthographicCamera::getTMin() const {
    return std::numeric_limits<float>::lowest();
}

OrthographicCamera::OrthographicCamera(const Vec3f &center, const Vec3f &direction, const Vec3f &up, float size) {
    _center = center;
    _direction = direction;
    _direction.Normalize();
    _up = up - _direction * _up.Dot3(_direction);
    _up.Normalize();
    Vec3f::Cross3(_horizontal, _direction, _up);
    _size = size;
}

PerspectiveCamera::PerspectiveCamera(const Vec3f &center, const Vec3f &direction, const Vec3f &up, float angleRadians) {
    _center = center;
    _direction = direction;
    _direction.Normalize();
    _up = up - _direction * _up.Dot3(_direction);
    _up.Normalize();
    Vec3f::Cross3(_horizontal, _direction, _up);
    _size = tanf(angleRadians / 2.f) * 2.f;
}

Ray PerspectiveCamera::generateRay(Vec2f point) {
    Vec3f rayDir = _direction + (point.x() - 0.5) * _size * _horizontal + (point.y() - 0.5) * _size * _up;
    rayDir.Normalize();
    return Ray(_center, rayDir);
}

float PerspectiveCamera::getTMin() const {
    return 0;
}
