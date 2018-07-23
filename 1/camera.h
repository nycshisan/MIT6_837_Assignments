//
// Created by Nycshisan on 2018/7/23.
//

#ifndef ASSIGNMENTS_CAMERA_H
#define ASSIGNMENTS_CAMERA_H

#include "vectors.h"
#include "ray.h"

class Camera {
public:
    virtual Ray generateRay(Vec2f point) = 0;
    virtual float getTMin() const = 0;

    virtual ~Camera() = default;
};

class OrthographicCamera : public Camera {
public:
    OrthographicCamera(const Vec3f &center, const Vec3f &direction, const Vec3f &up, float size);

    Ray generateRay(Vec2f point) override;
    float getTMin() const override;

    ~OrthographicCamera() override;
};

#endif //ASSIGNMENTS_CAMERA_H
