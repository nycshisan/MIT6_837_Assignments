//
// Created by Nycshisan on 2018/8/4.
//

#include "surface.h"

#include <OpenGL/gl.h>

SurfaceOfRevolution::SurfaceOfRevolution(Curve *c) : Surface(c->getNumVertices()) {
    _c = std::shared_ptr<Curve>(c);
}

void SurfaceOfRevolution::Paint(ArgParser *args) {
    _c->Paint(args);
}

TriangleMesh *SurfaceOfRevolution::OutputTriangles(ArgParser *args) {
    auto windows = _c->getWindows();
    auto *net = new TriangleNet(args->curve_tessellation * int(windows.size()), args->revolution_tessellation);
    for (int spinIndex = 0; spinIndex <= args->revolution_tessellation; ++spinIndex) {
        float spinStep = float(M_PI) * 2.f / args->revolution_tessellation * spinIndex;
        float spinSin = sinf(spinStep), spinCos = cosf(spinStep);
        int i = 0;
        for (int windowIndex = 0; windowIndex < windows.size(); ++windowIndex) {
            auto &window = windows[windowIndex];
            int pointIndex = windowIndex == 0 ? 0 : 1; // add the first vertex for the first window (curve segment)
            for (; pointIndex <= args->curve_tessellation; ++pointIndex) {
                auto v = window.getPointAtT(1.f / args->curve_tessellation * pointIndex);
                v.Set(v.x() * spinCos, v.y(), v.x() * spinSin);
                net->SetVertex(i++, spinIndex, v);
            }
        }
        assert(i == args->curve_tessellation * int(windows.size()) + 1);
    }
    return net;
}

void SurfaceOfRevolution::OutputBezier(FILE *file) {
    fprintf(file, "surface_of_revolution\n");
    _c->OutputBezier(file);
}

void SurfaceOfRevolution::OutputBSpline(FILE *file) {
    fprintf(file, "surface_of_revolution\n");
    _c->OutputBSpline(file);
}

BezierPatch::BezierPatch() : Surface(16) {
    _ctrlCurves = std::shared_ptr<BezierCurve>(new BezierCurve[4], [](BezierCurve *p){ delete[] p; });
    for (int i = 0; i < 4; ++i) {
        _ctrlCurves.get()[i] = BezierCurve(4);
    }
}

void BezierPatch::set(int i, const Vec3f &v) {
    _ctrlCurves.get()[i / 4].set(i % 4, v);
}

void BezierPatch::Paint(ArgParser *args) {
    CurveWindow ctrlWindows[4];
    for (int i = 0; i < 4; ++i) {
        ctrlWindows[i] = _ctrlCurves.get()[i].getWindows()[0];
    }

    ArgParser fakeArgs = *args;
    fakeArgs.curve_tessellation = args->patch_tessellation;

    int tess = args->patch_tessellation;
    for (int i = 0; i <= tess; ++i) {
        float t = i * 1.f / tess;
        BezierCurve bezier(4);
        for (int j = 0; j < 4; ++j) {
            bezier.set(j, ctrlWindows[j].getPointAtT(t));
        }
        bezier.Paint(&fakeArgs);
    }

//    // draw control points
//    glColor3f(1.f, 0.f, 0.f);
//    glPointSize(4.f);
//    glBegin(GL_POINTS);
//    for (auto &p: _ctrlPoints) {
//        glVertex3fv(p.GetData());
//    }
//    glEnd();
}

TriangleMesh *BezierPatch::OutputTriangles(ArgParser *args) {
    int tess = args->patch_tessellation;
    auto *net = new TriangleNet(tess, tess);

    CurveWindow ctrlWindows[4];
    for (int i = 0; i < 4; ++i) {
        ctrlWindows[i] = _ctrlCurves.get()[i].getWindows()[0];
    }

    for (int i = 0; i <= tess; ++i) {
        float t = i * 1.f / tess;
        BezierCurve bezier(4);
        for (int j = 0; j < 4; ++j) {
            bezier.set(j, ctrlWindows[j].getPointAtT(t));
        }
        auto window = bezier.getWindows()[0];
        for (int j = 0; j <= tess; ++j) {
            float s = j * 1.f / tess;
            net->SetVertex(i, j, window.getPointAtT(s));
        }
    }

    return net;
}
