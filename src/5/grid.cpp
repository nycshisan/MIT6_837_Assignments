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

static float _err = 1e-3f;
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
    auto bbStride = _bbMax - _bbMin;
    _stepX = bbStride.x() / _nx; _stepY = bbStride.y() / _ny; _stepZ = bbStride.z() / _nz;

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

        while (mi.i >= 0 && mi.i < _nx &&
               mi.j >= 0 && mi.j < _ny &&
               mi.k >= 0 && mi.k < _nz) {
            if (!cells[mi.i][mi.j][mi.k].empty()) {
                mi.hit = true;
                if (!firstHit) {
                    tHit = mi.tmin;
                    nHit = mi.normal_to_cell;
                    nObjects = static_cast<int>(cells[mi.i][mi.j][mi.k].size());
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
                        v[m] = _bbMin + Vec3f(_stepX * (i + ((m & 0x4u) >> 2u)), _stepY * (j + ((m & 0x2u) >> 1u)), _stepZ * (k + (m & 0x1u)));
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
    mi.sign_x = r.getDirection().x() > 0.f ? 1 : -1;
    mi.sign_y = r.getDirection().y() > 0.f ? 1 : -1;
    mi.sign_z = r.getDirection().z() > 0.f ? 1 : -1;

    const auto &startPoint = r.pointAtParameter(tmin);

    mi.d_tx = _stepX / r.getDirection().x() * mi.sign_x;
    mi.d_ty = _stepY / r.getDirection().y() * mi.sign_y;
    mi.d_tz = _stepZ / r.getDirection().z() * mi.sign_z;

    Vec3f hitPoint;
    // store the entered face
    char enteredFaceAxis = 0, enteredFacePositive = 0;

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
        char enteredFace[2][6] = {
                {'x', 'x', 'y', 'y', 'z', 'z'},
                {'-', '+', '-', '+', '-', '+'}
        };
        Hit shallowestHit;
        for (int m = 0; m < 6; ++m) {
            if (validHits[m]) {
                if (h[m].getT() < shallowestHit.getT()) {
                    shallowestHit = h[m];
                    mi.normal_to_cell = planeNormals[m];
                    // store the entered face information
                    enteredFaceAxis = enteredFace[0][m];
                    enteredFacePositive = enteredFace[1][m];
                }
            }
        }
        hitPoint = shallowestHit.getIntersectionPoint();
        mi.tmin = shallowestHit.getT();
    }

    // compute the initial values for marching info
    getGridCellIndex(hitPoint, mi.i, mi.j, mi.k);
    int next_i = mi.i + (mi.sign_x + 1) / 2;
    int next_j = mi.j + (mi.sign_y + 1) / 2;
    int next_k = mi.k + (mi.sign_z + 1) / 2;
    float next_x = _bbMin.x() + next_i * _stepX;
    float next_y = _bbMin.y() + next_j * _stepY;
    float next_z = _bbMin.z() + next_k * _stepZ;
    mi.t_next_x = (next_x - r.getOrigin().x()) / r.getDirection().x();
    mi.t_next_y = (next_y - r.getOrigin().y()) / r.getDirection().y();
    mi.t_next_z = (next_z - r.getOrigin().z()) / r.getDirection().z();

    // add help faces
    if (mi.hit)
        _addEnteredCell(mi.i, mi.j, mi.k, enteredFaceAxis, enteredFacePositive);
}

void Grid::_addEnteredCell(int i, int j, int k, char axis, char positive) {
    assert(axis); assert(positive);

    if (i < 0 || i >= _nx ||
        j < 0 || j >= _ny ||
        k < 0 || k >= _nz)
        return;

    Vec3f v[8];
    for (unsigned int m = 0; m < 8; ++m) {
        v[m] = _bbMin + Vec3f(_stepX * (i + ((m & 0x4u) >> 2u)), _stepY * (j + ((m & 0x2u) >> 1u)), _stepZ * (k + (m & 0x1u)));
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
    int m = 0;
    switch (axis) {
        case 'x':
            m = 0; break;
        case 'y':
            m = 2; break;
        case 'z':
            m = 4; break;
        default:
            assert(0);
    }
    if (positive == '+')
        ++m;
    RayTree::AddEnteredFace(v[_sideIndex[m][0]], v[_sideIndex[m][1]], v[_sideIndex[m][2]], v[_sideIndex[m][3]],
                            n[m], _getCellMaterial(_crtColorIndex % SchemaColorNumber));
    ++_crtColorIndex;
}

void Grid::_nextCell(MarchingInfo &mi) {
    char smallestTAxis;
    if (mi.t_next_x < mi.t_next_y) {
        if (mi.t_next_x < mi.t_next_z) {
            // t_next_x is the smallest t_next
            smallestTAxis = 'x';
        } else {
            // t_next_z is the smallest t_next
            smallestTAxis = 'z';
        }
    } else if (mi.t_next_y < mi.t_next_z) {
        // t_next_y is the smallest t_next
        smallestTAxis = 'y';
    } else {
        // t_next_z is the smallest t_next
        smallestTAxis = 'z';
    }
    char enteredFacePositives [2] = { '+', '-' };
    char enteredFacePositive = 0;
    switch (smallestTAxis) {
        case 'x':
            mi.i += mi.sign_x;
            mi.tmin = mi.t_next_x;
            mi.t_next_x += mi.d_tx;
            mi.normal_to_cell = Vec3f(1.f, 0.f, 0.f) * mi.sign_x;
            enteredFacePositive = enteredFacePositives[(mi.sign_x + 1) / 2];
            break;
        case 'y':
            mi.j += mi.sign_y;
            mi.tmin = mi.t_next_y;
            mi.t_next_y += mi.d_ty;
            mi.normal_to_cell = Vec3f(0.f, 1.f, 0.f) * mi.sign_y;
            enteredFacePositive = enteredFacePositives[(mi.sign_y + 1) / 2];
            break;
        case 'z':
            mi.k += mi.sign_z;
            mi.tmin = mi.t_next_z;
            mi.t_next_z += mi.d_tz;
            mi.normal_to_cell = Vec3f(0.f, 0.f, 1.f) * mi.sign_z;
            enteredFacePositive = enteredFacePositives[(mi.sign_z + 1) / 2];
            break;
    }
    _addEnteredCell(mi.i, mi.j, mi.k, smallestTAxis, enteredFacePositive);
}

Material *Grid::_getCellMaterial(int index) const {
    return const_cast<PhongMaterial*>(_materialSchema.data()) + index;
}

void Grid::getGridCellIndex(const Vec3f &p, int &i, int &j, int &k) {
    float fi = (p.x() - _bbMin.x()) / _stepX;
    float fj = (p.y() - _bbMin.y()) / _stepY;
    float fk = (p.z() - _bbMin.z()) / _stepZ;
    i = std::max(0, std::min(int(floorf(fi)), _nx - 1));
    j = std::max(0, std::min(int(floorf(fj)), _ny - 1));
    k = std::max(0, std::min(int(floorf(fk)), _nz - 1));
}
