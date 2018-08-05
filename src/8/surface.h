//
// Created by Nycshisan on 2018/8/4.
//

#ifndef ASSIGNMENTS_SURFACE_H
#define ASSIGNMENTS_SURFACE_H

#include <memory>

#include "spline.h"
#include "curve.h"

class Surface : public Spline {
public:
    explicit Surface(int nVertices) : Spline(nVertices) {}
};

class SurfaceOfRevolution : public Surface {
    std::shared_ptr<Curve> _c;

public:
    explicit SurfaceOfRevolution(Curve *c);

    void set(int i, const Vec3f &v) override { _c->set(i, v); }

    void Paint(ArgParser *args) override;

    int getNumVertices() override { return _c->getNumVertices(); }
    Vec3f getVertex(int i) override { return _c->getVertex(i); }

    void moveControlPoint(int selectedPoint, float x, float y) override { _c->moveControlPoint(selectedPoint, x, y); }
    void addControlPoint(int selectedPoint, float x, float y) override { _c->addControlPoint(selectedPoint, x, y); }
    void deleteControlPoint(int selectedPoint) override { _c->deleteControlPoint(selectedPoint); }

    void OutputBezier(FILE *file) override;
    void OutputBSpline(FILE *file) override;
    TriangleMesh* OutputTriangles(ArgParser *args) override;
};

class BezierPatch : public Surface {
    std::shared_ptr<BezierCurve> _ctrlCurves;

public:
    BezierPatch();

    void set(int i, const Vec3f &v) override;

    void Paint(ArgParser *args) override;

    TriangleMesh* OutputTriangles(ArgParser *args) override;
};

#endif //ASSIGNMENTS_SURFACE_H
