//
// Created by Nycshisan on 2018/8/4.
//

#ifndef ASSIGNMENTS_SPLINE_H
#define ASSIGNMENTS_SPLINE_H

#include <vector>

#include "arg_parser.h"
#include "vectors.h"
#include "triangle_mesh.h"

class Spline {
protected:
    int _nVertices;
    std::vector<Vec3f> _ctrlPoints;

public:
    explicit Spline(int nVertices); ;

    virtual void set(int i, const Vec3f &v);

    // FOR VISUALIZATION
    virtual void Paint(ArgParser *args) { assert(0); }

    // FOR CONVERTING BETWEEN SPLINE TYPES
    virtual void OutputBezier(FILE *file) { assert(0); }
    virtual void OutputBSpline(FILE *file) { assert(0); }

    // FOR CONTROL POINT PICKING
    virtual int getNumVertices() { return _nVertices; }
    virtual Vec3f getVertex(int i) { return _ctrlPoints[i]; }

    // FOR EDITING OPERATIONS
    virtual void moveControlPoint(int selectedPoint, float x, float y);
    virtual void addControlPoint(int selectedPoint, float x, float y);
    virtual void deleteControlPoint(int selectedPoint);

    // FOR GENERATING TRIANGLES
    virtual TriangleMesh* OutputTriangles(ArgParser *args) { assert(0); return nullptr; }
};

#endif //ASSIGNMENTS_SPLINE_H
