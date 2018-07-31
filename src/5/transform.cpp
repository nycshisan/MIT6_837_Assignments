//
// Created by Nycshisan on 2018/7/23.
//

#include "transform.h"

Transform::Transform(const Matrix &mat, Object3D *object) {
    _mat = mat;
    _invMat = _mat;
    int r = _invMat.Inverse();
    assert(r);
    _invTpMat = _invMat;
    _invTpMat.Transpose();
    _object = object;

    auto objectBoundingBox = object->getBoundingBox();
    Vec3f objectBoundingMinMax[2] = { objectBoundingBox->getMin(), objectBoundingBox->getMax() };

    Vec3f boundingVertexes[8];

    for (unsigned i = 0; i < 8; ++i) {
        auto x = objectBoundingMinMax[i & 0x1u].x();
        auto y = objectBoundingMinMax[(i & 0x2u) >> 1u].y();
        auto z = objectBoundingMinMax[(i & 0x4u) >> 2u].z();
        Vec3f objectBoundingVertex(x, y, z);
        _mat.Transform(objectBoundingVertex);
        boundingVertexes[i] = objectBoundingVertex;
    }

    _boundingBox = std::make_shared<BoundingBox>(Vec3f(), Vec3f());
    for (const auto &boundingVertex : boundingVertexes) {
        _boundingBox->Extend(boundingVertex);
    }
}

bool Transform::intersect(const Ray &r, Hit &h, float tmin) {
    // transform ray direction from world space to object space
    Vec4f hRayDir(r.getDirection(), 0.f);
    Vec4f hTransformedRayDir = _invMat * hRayDir;
    Vec3f transformedRayDir = hTransformedRayDir.xyz();
    float transformedRayDirLength = transformedRayDir.Length();
    transformedRayDir.Normalize();
    // transform ray origin from world space to object space
    Vec4f hRayOrigin(r.getOrigin(), 1.f);
    Vec4f hTransformedRayOrigin = _invMat * hRayOrigin;
    hTransformedRayOrigin.DivideByW();
    Vec3f transformedRayOrigin = hTransformedRayOrigin.xyz();

    Ray transformedRay(transformedRayOrigin, transformedRayDir);
    Hit osHit;
    if (_object->intersect(transformedRay, osHit, tmin)) {
        // transform normal from object space to world space
        auto osNorm = osHit.getNormal();
        auto hOsNorm = Vec4f(osNorm, 0.f);
        auto hWsNorm = _invTpMat * hOsNorm;
        auto wsNorm = hWsNorm.xyz();
        wsNorm.Normalize();
        // transform depth from object space to world space
        float osT = osHit.getT();
        float wsT = osT / transformedRayDirLength;
        if (wsT >= tmin) {
            h.set(wsT, osHit.getMaterial(), wsNorm, r, osHit.getObjectType());
            return true;
        } else {
            return false;
        }
    } else {
        return false;
    };
}

void Transform::paint() {
    glPushMatrix();
    GLfloat *glMatrix = _mat.glGet();
    glMultMatrixf(glMatrix);
    delete[] glMatrix;
    _object->paint();
    glPopMatrix();
}
