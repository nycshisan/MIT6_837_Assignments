//
// Created by Nycshisan on 2018/7/23.
//

#include "sphere.h"

#include <cmath>

#include "grid.h"
#include "vectors.h"

static float _err = 1e-4f;

Sphere::Sphere(const Vec3f &center, float radius, Material *material) {
    _center = center;
    _radius = radius;
    _m = material;

    Vec3f boundingRadius(radius, radius, radius);
    Vec3f boundingMin = center - boundingRadius, boundingMax = center + boundingRadius;
    _boundingBox = std::make_shared<BoundingBox>(boundingMin, boundingMax);
}

bool Sphere::intersect(const Ray &r, Hit &h, float tmin) {
    const auto &origin = r.getOrigin();
    auto dOrigin = origin - _center;
    auto squareLenOrigin = dOrigin.Dot3(dOrigin);
    auto squareRadius = _radius * _radius;
    auto dFootPointToOrigin = -dOrigin.Dot3(r.getDirection());
    auto squareDistRay = squareLenOrigin - dFootPointToOrigin * dFootPointToOrigin;
    if (squareDistRay > squareRadius) {
        // no hit
        return false;
    }
    auto squareDistIntersectionToFootPoint = squareRadius - squareDistRay;
    auto dIntersectionToOrigin_1 = dFootPointToOrigin + sqrt(squareDistIntersectionToFootPoint);
    auto dIntersectionToOrigin_2 = dFootPointToOrigin - sqrt(squareDistIntersectionToFootPoint);
    auto dIntersectionToOriginMin = std::min(dIntersectionToOrigin_1, dIntersectionToOrigin_2);
    auto dIntersectionToOriginMax = std::max(dIntersectionToOrigin_1, dIntersectionToOrigin_2);
    float t;
    if (dIntersectionToOriginMin >= tmin) {
        t = dIntersectionToOriginMin;
    } else if (dIntersectionToOriginMax >= tmin) {
        t = dIntersectionToOriginMax;
    } else {
        return false;
    }
    auto intersection = r.pointAtParameter(t);
    auto normal = intersection - _center;
    normal.Normalize();
    h.set(t, _m, normal, r, Hit::ObjectType::Sphere);
    return true;
}

int g_theta_steps = 10, g_phi_steps = 10;
bool g_gouraud = false;

void Sphere::paint() {
    _m->glSetMaterial();

    float theta_step = float(M_PI) * 2.f / g_theta_steps;
    float phi_step = float(M_PI) / g_phi_steps;
    float theta = 0.f, phi = 0.f; // phi - 天顶角; theta - 方位角
    float x0, y0, z0, x1, y1, z1, x2, y2, z2, x3, y3, z3, xn, yn, zn;

    glBegin(GL_QUADS);
    for (int iPhi = 0; iPhi < g_phi_steps; ++iPhi) {
        float nextPhi = phi + phi_step;
        for (int iTheta = 0; iTheta < g_theta_steps; ++iTheta) {
            float nextTheta = theta + theta_step;
            getXYZBySphereCoord(theta, phi, x0, y0, z0);
            getXYZBySphereCoord(theta, nextPhi, x1, y1, z1);
            getXYZBySphereCoord(nextTheta, nextPhi, x2, y2, z2);
            getXYZBySphereCoord(nextTheta, phi, x3, y3, z3);
            if (!g_gouraud) {
                Vec3f v0(x0, y0, z0), v1(x1, y1, z1), v2(x2, y2, z2), v3(x3, y3, z3);
                Vec3f norm = (v0 + v1 + v2 + v3) * (0.25f) - _center;
                norm.Get(xn, yn, zn);
                glNormal3f(xn, yn, zn);
            }
            if (g_gouraud)
                glNormal3f(x0 - _center.x(), y0 - _center.y(), z0 - _center.z());
            glVertex3f(x0, y0, z0);
            if (g_gouraud)
                glNormal3f(x1 - _center.x(), y1 - _center.y(), z1 - _center.z());
            glVertex3f(x1, y1, z1);
            if (g_gouraud)
                glNormal3f(x2 - _center.x(), y2 - _center.y(), z2 - _center.z());
            glVertex3f(x2, y2, z2);
            if (g_gouraud)
                glNormal3f(x3 - _center.x(), y3 - _center.y(), z3 - _center.z());
            glVertex3f(x3, y3, z3);

            theta = nextTheta;
        }
        phi = nextPhi;
    }
    glEnd();
}

void Sphere::getXYZBySphereCoord(float theta, float phi, float &x, float &y, float &z) {
    // The assignment use negative axis-z as forward, positive axis-y as up and positive axis-x as right
    float rsint = _radius * sinf(phi);
    float rcost = _radius * cosf(phi);
    x = rsint * cosf(theta) + _center.x();
    y = rcost + _center.y();
    z = -(rsint * sinf(theta)) + _center.z();
}

void Sphere::insertIntoGrid(Grid *g, Matrix *m) {
    auto bbMin = g->getBBMin(), bbMax = g->getBBMax();
    int nx, ny, nz; g->getN(nx, ny, nz);
    float stepX, stepY, stepZ; g->getStep(stepX, stepY, stepZ);
    float gridDiagLen = Vec3f(stepX, stepY, stepZ).Length() / 2.f;
    for (int i = 0; i < nx; ++i) {
        for (int j = 0; j < ny; ++j) {
            for (int k = 0; k < nz; ++k) {
                float x = i + 0.5f, y = j + 0.5f, z = k + 0.5f;
                Vec3f gridCenter = bbMin + Vec3f(x * stepX, y * stepY, z * stepZ);
                float dist = (gridCenter - _center).Length();
                if (dist < _radius + gridDiagLen + _err)
                    g->occupation[i][j][k].emplace_back(this);
            }
        }
    }
}