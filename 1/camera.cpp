//
// Created by Nycshisan on 2018/7/23.
//

#include "camera.h"

Ray OrthographicCamera::generateRay(Vec2f point) {
    return Ray();
}

float OrthographicCamera::getTMin() const {
    return 0;
}

OrthographicCamera::OrthographicCamera(const Vec3f &center, const Vec3f &direction, const Vec3f &up, float size) {

}

OrthographicCamera::~OrthographicCamera() {

}
