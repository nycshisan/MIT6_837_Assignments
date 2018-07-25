//
// Created by Nycshisan on 2018/7/23.
//

#include "camera.h"

#include <limits>

#include "matrix.h"

OrthographicCamera::OrthographicCamera(const Vec3f &center, const Vec3f &direction, const Vec3f &up, float size) {
    _center = center;
    _direction = direction;
    _direction.Normalize();
    _up = up - _direction * _up.Dot3(_direction);
    _up.Normalize();
    Vec3f::Cross3(_horizontal, _direction, _up);
    _horizontal.Normalize();
    _size = size;
}

Ray OrthographicCamera::generateRay(Vec2f point) {
    auto origin = _center + (point.x() - 0.5) * _size * _horizontal + (point.y() - 0.5) * _size * _up;
    return Ray(origin, _direction);
}

float OrthographicCamera::getTMin() const {
    return std::numeric_limits<float>::lowest();
}

PerspectiveCamera::PerspectiveCamera(const Vec3f &center, const Vec3f &direction, const Vec3f &up, float angleRadians) {
    _center = center;
    _direction = direction;
    _direction.Normalize();
    _up = up - _direction * _up.Dot3(_direction);
    _up.Normalize();
    Vec3f::Cross3(_horizontal, _direction, _up);
    _horizontal.Normalize();
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

/* Mostly codes are copied from course materials */

void OrthographicCamera::glInit(int w, int h) {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if (w > h)
        glOrtho(-_size/2.0, _size/2.0, -_size*(float)h/(float)w/2.0, _size*(float)h/(float)w/2.0, 0.5, 40.0);
    else
        glOrtho(-_size*(float)w/(float)h/2.0, _size*(float)w/(float)h/2.0, -_size/2.0, _size/2.0, 0.5, 40.0);
}

void OrthographicCamera::glPlaceCamera(void) {
    gluLookAt(_center.x(), _center.y(), _center.z(),
              _center.x()+_direction.x(), _center.y()+_direction.y(), _center.z()+_direction.z(),
              _up.x(), _up.y(), _up.z());
}

// ====================================================================
// dollyCamera: Move camera along the direction vector
// ====================================================================

void OrthographicCamera::dollyCamera(float dist) {
    _center += _direction*dist;
}

// ====================================================================
// truckCamera: Translate camera perpendicular to the direction vector
// ====================================================================

void OrthographicCamera::truckCamera(float dx, float dy) {
    _center += _horizontal*dx + _up*dy;
}

// ====================================================================
// rotateCamera: Rotate around the up and horizontal vectors
// ====================================================================

void OrthographicCamera::rotateCamera(float rx, float ry) {
    // Don't let the model flip upside-down (There is a singularity
    // at the poles when 'up' and 'direction' are aligned)
    float tiltAngle = acos(_up.Dot3(_direction));
    if (tiltAngle-ry > 3.13)
        ry = tiltAngle - 3.13;
    else if (tiltAngle-ry < 0.01)
        ry = tiltAngle - 0.01;

    Matrix rotMat = Matrix::MakeAxisRotation(up, rx);
    rotMat *= Matrix::MakeAxisRotation(horizontal, ry);

    rotMat.Transform(center);
    rotMat.TransformDirection(direction);

    // ===========================================
    // ASSIGNMENT 3: Fix any other affected values
    // ===========================================


}

// ====================================================================
// ====================================================================

// ====================================================================
// Create a perspective camera with the appropriate dimensions that
// crops or stretches in the x-dimension as necessary
// ====================================================================

void PerspectiveCamera::glInit(int w, int h)
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(angle*180.0/3.14159, (float)w/float(h), 0.5, 40.0);
}

// ====================================================================
// Place a perspective camera within an OpenGL scene
// ====================================================================

void PerspectiveCamera::glPlaceCamera(void)
{
    gluLookAt(center.x(), center.y(), center.z(),
              center.x()+direction.x(), center.y()+direction.y(), center.z()+direction.z(),
              up.x(), up.y(), up.z());
}

// ====================================================================
// dollyCamera, truckCamera, and RotateCamera
//
// Asumptions:
//  - up is really up (i.e., it hasn't been changed
//    to point to "screen up")
//  - up and direction are normalized
// Special considerations:
//  - If your constructor precomputes any vectors for
//    use in 'generateRay', you will likely to recompute those
//    values at athe end of the these three routines
// ====================================================================

// ====================================================================
// dollyCamera: Move camera along the direction vector
// ====================================================================

void PerspectiveCamera::dollyCamera(float dist)
{
    center += direction*dist;

    // ===========================================
    // ASSIGNMENT 3: Fix any other affected values
    // ===========================================


}

// ====================================================================
// truckCamera: Translate camera perpendicular to the direction vector
// ====================================================================

void PerspectiveCamera::truckCamera(float dx, float dy)
{
    Vec3f horizontal;
    Vec3f::Cross3(horizontal, direction, up);
    horizontal.Normalize();

    Vec3f screenUp;
    Vec3f::Cross3(screenUp, horizontal, direction);

    center += horizontal*dx + screenUp*dy;

    // ===========================================
    // ASSIGNMENT 3: Fix any other affected values
    // ===========================================


}

// ====================================================================
// rotateCamera: Rotate around the up and horizontal vectors
// ====================================================================

void PerspectiveCamera::rotateCamera(float rx, float ry)
{
    Vec3f horizontal;
    Vec3f::Cross3(horizontal, direction, up);
    horizontal.Normalize();

    // Don't let the model flip upside-down (There is a singularity
    // at the poles when 'up' and 'direction' are aligned)
    float tiltAngle = acos(up.Dot3(direction));
    if (tiltAngle-ry &gt; 3.13)
        ry = tiltAngle - 3.13;
    else if (tiltAngle-ry &lt; 0.01)
        ry = tiltAngle - 0.01;

    Matrix rotMat = Matrix::MakeAxisRotation(up, rx);
    rotMat *= Matrix::MakeAxisRotation(horizontal, ry);

    rotMat.Transform(center);
    rotMat.TransformDirection(direction);
    direction.Normalize();

    // ===========================================
    // ASSIGNMENT 3: Fix any other affected values
    // ===========================================


}

// ====================================================================
// =================