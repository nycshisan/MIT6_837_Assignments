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
