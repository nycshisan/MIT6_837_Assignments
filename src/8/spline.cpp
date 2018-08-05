//
// Created by Nycshisan on 2018/8/5.
//

#include "spline.h"

Spline::Spline(int nVertices) {
    _nVertices = nVertices;
    _ctrlPoints.resize(static_cast<unsigned long>(_nVertices));
}

void Spline::set(int i, const Vec3f &v) {
    _ctrlPoints[i] = v;
}

void Spline::moveControlPoint(int selectedPoint, float x, float y) {
    set(selectedPoint, Vec3f(x, y, 0.f));
}

void Spline::addControlPoint(int selectedPoint, float x, float y) {
    ++_nVertices;
    _ctrlPoints.resize(static_cast<unsigned long>(_nVertices));
    for (int i = _nVertices - 1; i > selectedPoint; --i) {
        _ctrlPoints[i] = _ctrlPoints[i - 1];
    }
    set(selectedPoint, Vec3f(x, y, 0.f));
}

void Spline::deleteControlPoint(int selectedPoint) {
    --_nVertices;
    for (int i = selectedPoint; i < _nVertices - 1; ++i) {
        _ctrlPoints[i] = _ctrlPoints[i + 1];
    }
    _ctrlPoints.resize(static_cast<unsigned long>(_nVertices));
}
