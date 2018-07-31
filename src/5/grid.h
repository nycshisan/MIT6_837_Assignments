//
// Created by Nycshisan on 2018/7/29.
//

#ifndef ASSIGNMENTS_GRID_H
#define ASSIGNMENTS_GRID_H

#include <vector>
#include <array>

#include "object3d.h"

class MarchingInfo {
public:
    float tmin;
    int i, j, k;
    float t_next_x, t_next_y, t_next_z, d_tx, d_ty, d_tz;
    int sign_x, sign_y, sign_z;
    Vec3f normal_to_cell;
    bool hit = false;
};

class Grid : public Object3D {
    static PhongMaterial GridMaterial;

    int _nx, _ny, _nz;
    Vec3f _bbMin, _bbMax;
    float _stepX, _stepY, _stepZ;

    void _initializeRayMarch(MarchingInfo &mi, const Ray &r, float tmin);
    void _addEnteredCell(int i, int j, int k, char axis, char positive);

    void _nextCell(MarchingInfo &info);
    Material *_getCellMaterial(int index) const;

    int _sideIndex[6][4] = {
        {0, 1, 3, 2},
        {4, 5, 7, 6},
        {0, 4, 5, 1},
        {2, 6, 7, 3},
        {0, 4, 6, 2},
        {1, 5, 7, 3}
    };

    static constexpr int SchemaColorNumber = 17;
    int _crtColorIndex = 0;
    std::array<PhongMaterial, SchemaColorNumber> _materialSchema;

public:
    std::vector<std::vector<std::vector<std::vector<Object3D*>>>> occupation;

    Grid(const std::shared_ptr<BoundingBox> &bb, int nx, int ny, int nz);
    bool intersect(const Ray &r, Hit &h, float tmin) override;
    void paint() override;

    void getN(int &nx, int &ny, int &nz);
    Vec3f getBBMin() { return _bbMin; }
    Vec3f getBBMax() { return _bbMax; }
    void getStep(float &x, float &y, float &z) {
        x = _stepX; y = _stepY; z = _stepZ;
    }

    void refreshColorSchema() { _crtColorIndex = 0; }
};

#endif //ASSIGNMENTS_GRID_H
