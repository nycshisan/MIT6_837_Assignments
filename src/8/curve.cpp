//
// Created by Nycshisan on 2018/8/4.
//

#include "curve.h"

#include <OpenGL/gl.h>

void Curve::Paint(ArgParser *args) {
    // draw the control polygon
    glColor3f(0.f, 0.f, 1.f);
    glLineWidth(2.f);
    glBegin(GL_LINE_STRIP);
    for (auto &p: _ctrlPoints) {
        glVertex3fv(p.GetData());
    }
    glEnd();

    // draw the actual curve
    _makeWindows();
    int tess = args->curve_tessellation;
    glColor3f(0.f, 1.f, 0.f);
    glLineWidth(3.f);
    glBegin(GL_LINE_STRIP);
    for (const auto &window: _windows) {
        for (int i = 0; i <= tess; ++i) {
            glVertex3fv(window.getPointAtT(1.f / tess * i).GetData());
        }
    }
    glEnd();

    // draw control points
    glColor3f(1.f, 0.f, 0.f);
    glPointSize(4.f);
    glBegin(GL_POINTS);
    for (auto &p: _ctrlPoints) {
        glVertex3fv(p.GetData());
    }
    glEnd();
}

void Curve::_makeWindow(int windowIndex, int beginPointIndex) {
    auto &window = _windows[windowIndex];
    window.B = getB();
    Vec4f v[4];
    for (int j = 0; j < 4; ++j) {
        v[j] = Vec4f(_ctrlPoints[beginPointIndex + j], 1.f);
    }
    window.G = Matrix(v);
}

void Curve::_output(FILE *file) {
    fprintf(file, "%s\nnum_vertices %d\n", _type.c_str(), _nVertices);
    for (const auto &p: _ctrlPoints) {
        fprintf(file, "%.2f %.2f %.2f\n", p.x(), p.y(), p.z());
    }
}

std::vector<CurveWindow> Curve::getWindows() {
    _makeWindows();
    return _windows;
}

Vec3f CurveWindow::getPointAtT(float t) const {
    auto _T = Vec4f(t * t * t, t * t, t, 1);
    (G * *B).Transform(_T);
    _T.DivideByW();
    return _T.xyz();
}

static float BezierBFloats[16] = {
        -1.f, 3.f, -3.f, 1.f,
        3.f, -6.f, 3.f, 0.f,
        -3.f, 3.f, 0.f, 0.f,
        1.f, 0.f, 0.f, 0.f
};
Matrix BezierCurve::_B = Matrix(BezierBFloats);

BezierCurve::BezierCurve(int nVertices) : Curve(nVertices) {
    _type = "bezier";
}

void BezierCurve::_makeWindows() {
    int nWindows = (_nVertices - 1) / 3;
    _windows.resize(static_cast<unsigned long>(nWindows));
    for (int i = 0; i < nWindows; ++i) {
        int beginPointIndex = 3 * i;
        _makeWindow(i, beginPointIndex);
    }
}

std::shared_ptr<BSplineCurve> BezierCurve::toBSpline() {
    assert(_nVertices == 4);
    _makeWindows();

    auto result = std::make_shared<BSplineCurve>(_nVertices);

    auto &window = _windows[0];
    auto BZG = window.G;
    auto BZB = *(this->getB());
    auto BSB = *(result->getB());
    assert(BSB.Inverse(1e-5f));
    auto BSG = BZG * BZB * BSB;

    for (int i = 0; i < 4; ++i) {
        float f[4];
        for (int j = 0; j < 4; ++j) {
            f[j] = BSG.Get(i, j);
        }
        Vec4f v(f[0], f[1], f[2], f[3]);
        v.DivideByW();
        result->set(i, Vec3f(v.x(), v.y(), v.z()));
    }

    return result;
}

void BezierCurve::OutputBezier(FILE *file) {
    _output(file);
}

void BezierCurve::OutputBSpline(FILE *file) {
    toBSpline()->OutputBSpline(file);
}

static float BSplineBFloats[16] = {
        -1.f, 3.f, -3.f, 1.f,
        3.f, -6.f, 0.f, 4.f,
        -3.f, 3.f, 3.f, 1.f,
        1.f, 0.f, 0.f, 0.f
};
Matrix BSplineCurve::_B = Matrix(BSplineBFloats) * (1.f / 6.f);

BSplineCurve::BSplineCurve(int nVertices) : Curve(nVertices) {
    _type = "bspline";
}

void BSplineCurve::_makeWindows() {
    int nWindows = _nVertices - 3;
    _windows.resize(static_cast<unsigned long>(nWindows));
    for (int i = 0; i < nWindows; ++i) {
        int beginPointIndex = i;
        _makeWindow(i, beginPointIndex);
    }
}

std::shared_ptr<BezierCurve> BSplineCurve::toBezier() {
    assert(_nVertices == 4);
    _makeWindows();

    auto result = std::make_shared<BezierCurve>(_nVertices);

    auto &window = _windows[0];
    auto BSG = window.G;
    auto BSB = *(this->getB());
    auto BZB = *(result->getB());
    assert(BZB.Inverse(1e-5f));
    auto BZG = BSG * BSB * BZB;

    for (int i = 0; i < 4; ++i) {
        float f[4];
        for (int j = 0; j < 4; ++j) {
            f[j] = BZG.Get(i, j);
        }
        Vec4f v(f[0], f[1], f[2], f[3]);
        v.DivideByW();
        result->set(i, Vec3f(v.x(), v.y(), v.z()));
    }

    return result;
}

void BSplineCurve::OutputBezier(FILE *file) {
    toBezier()->OutputBezier(file);
}

void BSplineCurve::OutputBSpline(FILE *file) {
    _output(file);
}
