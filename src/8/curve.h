//
// Created by Nycshisan on 2018/8/4.
//

#ifndef ASSIGNMENTS_CURVE_H
#define ASSIGNMENTS_CURVE_H

#include <memory>
#include <string>

#include "spline.h"
#include "matrix.h"

class CurveWindow {
public:
    Matrix G, *B;

    Vec3f getPointAtT(float t) const;
};

class Curve : public Spline {
protected:
    std::vector<CurveWindow> _windows;
    void _makeWindow(int windowIndex, int beginPointIndex);

    std::string _type;
    void _output(FILE *file);

public:
    explicit Curve(int nVertices) : Spline(nVertices) {}

    virtual Matrix *getB() = 0;
    std::vector<CurveWindow> getWindows();

    void Paint(ArgParser *args) override;

    virtual void _makeWindows() = 0;
};

class BezierCurve;
class BSplineCurve;

class BezierCurve : public Curve {
    static Matrix _B;

    void _makeWindows() override;

    friend class BezierPatch;
    BezierCurve() : Curve(0) {}

public:
    explicit BezierCurve(int nVertices);

    Matrix *getB() override { return &_B; };

    std::shared_ptr<BSplineCurve> toBSpline();

    void OutputBezier(FILE *file) override;
    void OutputBSpline(FILE *file) override;


};

class BSplineCurve : public Curve {
    static Matrix _B;

    void _makeWindows() override;

public:
    explicit BSplineCurve(int nVertices);

    Matrix *getB() override { return &_B; };

    std::shared_ptr<BezierCurve> toBezier();

    void OutputBezier(FILE *file) override;
    void OutputBSpline(FILE *file) override;
};

#endif //ASSIGNMENTS_CURVE_H
