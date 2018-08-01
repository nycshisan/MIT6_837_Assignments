//
// Created by Nycshisan on 2018/7/29.
//

#include "grid.h"

#include <cmath>
#include <functional>

#include "material.h"
#include "plane.h"
#include "rayTree.h"
#include "hsl.h"
#include "boundingbox.h"

static float _err = 1e-5f;
constexpr int Grid::SchemaColorNumber;

PhongMaterial Grid::GridMaterial(Vec3f(1.f, 1.f, 1.f), Vec3f(), 0.f, Vec3f(), Vec3f(), 1.f);

Grid::Grid(const std::shared_ptr<BoundingBox> &bb, int nx, int ny, int nz) {
    _type = ObjectType::GridObject;

    _bb = bb;
    cells.resize(static_cast<unsigned long>(nx));
    for (int i = 0; i < nx; ++i) {
        cells[i].resize(static_cast<unsigned long>(ny));
        for (int j = 0; j < ny; ++j) {
            cells[i][j].resize(static_cast<unsigned long>(nz));
            for (int k = 0; k < nz; ++k) {
                cells[i][j][k].clear();
            }
        }
    }
    _nx = nx; _ny = ny; _nz = nz;

    _bbMin = _bb->getMin(); _bbMax = _bb->getMax();
    _step = (_bbMax - _bbMin) * Vec3f(1.f / _nx, 1.f / _ny, 1.f / _nz);


    _m = &GridMaterial;

    // init material schema
    for (int i = 1; i < SchemaColorNumber; ++i) {
        HSLColor hsl(360.f - 360.f / (SchemaColorNumber - 1) * (i + 1), 1.f, 0.5f);
        _materialSchema[i] = PhongMaterial(hsl.toRGBColor(), Vec3f(), 0.f, Vec3f(), Vec3f(), 1.f);
    }
    _materialSchema[0] = GridMaterial;
}

void Grid::getN(int &nx, int &ny, int &nz) {
    nx = _nx; ny = _ny; nz = _nz;
}

bool Grid::intersect(const Ray &r, Hit &h, float tmin) {
    MarchingInfo mi;
    _initializeRayMarch(mi, r, tmin);

    if (mi.hit) {
        // hit the bounding box
        mi.hit = false;
        bool firstHit = false;
        float tHit = 0.f;
        Vec3f nHit;
        int nObjects = 0;

        while (mi.index[0] >= 0 && mi.index[0] < _nx &&
               mi.index[1] >= 0 && mi.index[1] < _ny &&
               mi.index[2] >= 0 && mi.index[2] < _nz) {
            if (!cells[mi.index[0]][mi.index[1]][mi.index[2]].empty()) {
                mi.hit = true;
                if (!firstHit) {
                    tHit = mi.tmin;
                    nHit = mi.normal_to_cell;
                    nObjects = static_cast<int>(cells[mi.index[0]][mi.index[1]][mi.index[2]].size());
                    firstHit = true;
                }
            }
            _nextCell(mi);
        }

        if (mi.hit) {
            nHit.Negate();
            h.set(tHit, _getCellMaterial(std::min(nObjects, SchemaColorNumber) - 1), nHit, r, ObjectType::GridObject);
            return true;
        }
    }
    return false;
}

void Grid::paint() {
    for (unsigned i = 0; i < _nx; ++i) {
        for (unsigned j = 0; j < _ny; ++j) {
            for (unsigned k = 0; k < _nz; ++k) {
                if (!cells[i][j][k].empty()) {
                    auto nObjects = static_cast<int>(cells[i][j][k].size());
                    _materialSchema[std::min(nObjects, SchemaColorNumber) - 1].glSetMaterial();

                    Vec3f v[8];
                    for (unsigned int m = 0; m < 8; ++m) {
                        v[m] = _bbMin + _step * Vec3f(i + ((m & 0x4u) >> 2u), j + ((m & 0x2u) >> 1u), k + (m & 0x1u));
                    }

                    auto addPointsToGL = [v, this](int i) {
                        for (int m = 0; m < 4; ++m)
                            glVertex3fv(v[this->_sideIndex[i][m]].GetData());
                    };

                    glBegin(GL_QUADS);
                    // left side
                    glNormal3f(-1.f, 0.f, 0.f);
                    addPointsToGL(0);
                    // right side
                    glNormal3f(-1.f, 0.f, 0.f);
                    addPointsToGL(1);
                    // down side
                    glNormal3f(0.f, -1.f, 0.f);
                    addPointsToGL(2);
                    // up side
                    glNormal3f(0.f, -1.f, 0.f);
                    addPointsToGL(3);
                    // back side
                    glNormal3f(0.f, 0.f, 1.f);
                    addPointsToGL(4);
                    // forward side
                    glNormal3f(0.f, 0.f, 1.f);
                    addPointsToGL(5);
                    glEnd();
                };
            }
        }
    }
}

void Grid::_initializeRayMarch(MarchingInfo &mi, const Ray &r, float tmin) {
    for (int i = 0; i < 3; ++i) {
        mi.sign[i] = r.getDirection()[i] > 0.f ? 1 : -1;
        mi.d_t[i] = _step[i] / r.getDirection()[i] * mi.sign[i];
    }

    const auto &startPoint = r.pointAtParameter(tmin);

    Vec3f hitPoint;
    // store the entered face
    int enteredFaceAxis = -1, enteredFacePositive = 0;

    if (startPoint.x() > _bbMin.x() && startPoint.x() < _bbMax.x() &&
        startPoint.y() > _bbMin.y() && startPoint.y() < _bbMax.y() &&
        startPoint.z() > _bbMin.z() && startPoint.z() < _bbMax.z()) {
        // inside the box
        mi.hit = true;
        hitPoint = startPoint;
    } else {
        // outside the box
        // construct the facets
        Vec3f v[8];
        float x[2] = { _bbMin.x(), _bbMax.x() };
        float y[2] = { _bbMin.y(), _bbMax.y() };
        float z[2] = { _bbMin.z(), _bbMax.z() };
        for (unsigned int m = 0; m < 8; ++m) {
            v[m].SetX(x[(m & 0x4u) >> 2u]);
            v[m].SetY(y[(m & 0x2u) >> 1u]);
            v[m].SetZ(z[(m & 0x1u)]);
        }
        // check intersection on the facets
        mi.hit = false;
        Hit h[6];
        bool validHits[6] = { false, false, false, false, false, false };
        Plane planes[6] = { Plane(v[0], v[1], v[3], _m), // left side
                                  Plane(v[4], v[5], v[7], _m), // right side
                                  Plane(v[0], v[4], v[5], _m), // down side
                                  Plane(v[2], v[6], v[7], _m), // up side
                                  Plane(v[0], v[4], v[6], _m), // back side
                                  Plane(v[1], v[5], v[7], _m), // forward side
        };
        Vec3f planeNormals[6] = { Vec3f(1.f, 0.f, 0.f), // left side
                                  Vec3f(-1.f, 0.f, 0.f), // right side
                                  Vec3f(0.f, 1.f, 0.f), // down side
                                  Vec3f(0.f, -1.f, 0.f), // up side
                                  Vec3f(0.f, 0.f, 1.f), // back side
                                  Vec3f(0.f, 0.f, -1.f) // forward side
        };
        Vec3f intersect;

        auto bbContains = [this](const Vec3f &p) -> bool {
            return p.x() >= this->_bbMin.x() - _err && p.x() <= this->_bbMax.x() + _err &&
                   p.y() >= this->_bbMin.y() - _err && p.y() <= this->_bbMax.y() + _err &&
                   p.z() >= this->_bbMin.z() - _err && p.z() <= this->_bbMax.z() + _err;
        };

        for (int m = 0; m < 6; ++m) {
            if (planes[m].intersect(r, h[m], tmin)) {
                intersect = h[m].getIntersectionPoint();
                if (bbContains(intersect)) {
                    validHits[m] = true;
                    mi.hit = true;
                }
            }
        }

        // get the shallowest intersect
        Hit shallowestHit;
        for (int m = 0; m < 6; ++m) {
            if (validHits[m]) {
                if (h[m].getT() < shallowestHit.getT()) {
                    shallowestHit = h[m];
                    mi.normal_to_cell = planeNormals[m];
                    // store the entered face information
                    enteredFaceAxis = m / 2;
                    enteredFacePositive = m % 2;
                }
            }
        }
        hitPoint = shallowestHit.getIntersectionPoint();
        mi.tmin = shallowestHit.getT();
    }

    // compute the initial values for marching info
    getGridCellIndex(hitPoint, mi.index);
    for (int i = 0; i < 3; ++i) {
        int nextIndex = mi.index[i] + (mi.sign[i] + 1) / 2;
        float nextPos = _bbMin[i] + nextIndex * _step[i];
        mi.t_next[i] = (nextPos - r.getOrigin()[i]) / r.getDirection()[i];
    }

    // add help faces
    if (mi.hit)
        _addEnteredCell(mi.index[0], mi.index[1], mi.index[2], enteredFaceAxis, enteredFacePositive);
}

void Grid::_addEnteredCell(int i, int j, int k, int index, int positive) {
    if (i < 0 || i >= _nx ||
        j < 0 || j >= _ny ||
        k < 0 || k >= _nz)
        return;

    Vec3f v[8];
    for (unsigned int m = 0; m < 8; ++m) {
        v[m] = _bbMin + _step * Vec3f(i + ((m & 0x4u) >> 2u), j + ((m & 0x2u) >> 1u), k + (m & 0x1u));
    }
    Vec3f n[6] = { Vec3f(-1.f, 0.f, 0.f), // left side
                   Vec3f(-1.f, 0.f, 0.f), // right side
                   Vec3f(0.f, -1.f, 0.f), // down side
                   Vec3f(0.f, -1.f, 0.f), // up side
                   Vec3f(0.f, 0.f, 1.f), // back side
                   Vec3f(0.f, 0.f, 1.f) // forward side
    };

    // hit cell
    for (int m = 0; m < 6; ++m) {
        RayTree::AddHitCellFace(v[_sideIndex[m][0]], v[_sideIndex[m][1]], v[_sideIndex[m][2]], v[_sideIndex[m][3]],
                                n[m], _getCellMaterial(_crtColorIndex % SchemaColorNumber));
    }

    // enter side
    int m = index * 2 + positive;
    RayTree::AddEnteredFace(v[_sideIndex[m][0]], v[_sideIndex[m][1]], v[_sideIndex[m][2]], v[_sideIndex[m][3]],
                            n[m], _getCellMaterial(_crtColorIndex % SchemaColorNumber));
    ++_crtColorIndex;
}

void Grid::_nextCell(MarchingInfo &mi) {
    int smallestIndex = -1;
    float smallestT = std::numeric_limits<float>::max();
    for (int m = 0; m < 3; ++m) {
        if (mi.t_next[m] >= mi.tmin && mi.t_next[m] < smallestT) {
            smallestT = mi.t_next[m];
            smallestIndex = m;
        }
    }

    Vec3f normals[3] = { Vec3f(1.f, 0.f, 0.f), Vec3f(0.f, 1.f, 0.f), Vec3f(0.f, 0.f, 1.f) };
    mi.index[smallestIndex] += mi.sign[smallestIndex];
    mi.tmin = mi.t_next[smallestIndex];
    mi.t_next[smallestIndex] += mi.d_t[smallestIndex];
    mi.normal_to_cell = normals[smallestIndex] * mi.sign[smallestIndex];
    int enteredFacePositive = (1 - mi.sign[smallestIndex]) / 2;

    _addEnteredCell(mi.index[0], mi.index[1], mi.index[2], smallestIndex, enteredFacePositive);
}

Material *Grid::_getCellMaterial(int index) const {
    return const_cast<PhongMaterial*>(_materialSchema.data()) + index;
}

void Grid::getGridCellIndex(const Vec3f &p, int index[3]) {
    int n[3] = { _nx, _ny, _nz };
    for (int i = 0; i < 3; ++i) {
        float fi = (p[i] - _bbMin[i]) / _step[i];
        index[i] = std::max(0, std::min(int(floorf(fi)), n[i] - 1));
    }
}
