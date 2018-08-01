//
// Created by Nycshisan on 2018/7/23.
//

#include "transform.h"

#include "boundingbox.h"
#include "ray.h"
#include "hit.h"
#include "triangle.h"

Transform::Transform(const Matrix &mat, Object3D *object) {
    _type = object->getObjectType();

    _mat = mat;
    _invMat = _mat;
    int r = _invMat.Inverse();
    assert(r);
    _invTpMat = _invMat;
    _invTpMat.Transpose();
    _object = object;

    _bb = std::shared_ptr<BoundingBox>::make_shared(TransformBoundingBox(*_object, _mat));
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

void Transform::insertIntoGrid(Grid *g, Matrix *m) {
    Matrix thisM = _mat;
    if (m != nullptr) {
        thisM = *m * thisM;
    }
    _object->insertIntoGrid(g, &thisM);

}

BoundingBox Transform::TransformBoundingBox(const Object3D &object, const Matrix &m) {
    float maxf = std::numeric_limits<float>::max(), minf = std::numeric_limits<float>::lowest();
    BoundingBox transformedBB(Vec3f(maxf, maxf, maxf), Vec3f(minf, minf, minf));

    if (object.getObjectType() != ObjectType::TriangleObject) {
        Vec3f bbMinMax[2] = {object.getBoundingBox()->getMin(), object.getBoundingBox()->getMax()};

        Vec3f bbVertexes[8];
        for (unsigned i = 0; i < 8; ++i) {
            auto x = bbMinMax[i & 0x1u].x();
            auto y = bbMinMax[(i & 0x2u) >> 1u].y();
            auto z = bbMinMax[(i & 0x4u) >> 2u].z();
            Vec3f bbVertex(x, y, z);
            m.Transform(bbVertex);
            bbVertexes[i] = bbVertex;
        }

        for (const auto &bbVertex : bbVertexes) {
            transformedBB.Extend(bbVertex);
        }
    } else {
        Vec3f bbVertexes[3];
        const auto &tri = dynamic_cast<const Triangle &>(object);

        for (int i = 0; i < 3; ++i) {
            bbVertexes[i] = tri.getVertex(i);
            m.Transform(bbVertexes[i]);
        }

        for (const auto &bbVertex : bbVertexes) {
            transformedBB.Extend(bbVertex);
        }
    }

    return transformedBB;
}
