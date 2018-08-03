//
// Created by Nycshisan on 2018/7/29.
//

#ifndef ASSIGNMENTS_GRID_H
#define ASSIGNMENTS_GRID_H

#include <vector>
#include <array>
#include <set>

#include "object3d.h"
#include "material.h"

class MarchingInfo {
public:
    float tmin;
    int index[3];
    Vec3f t_next, d_t;
    int sign[3];
    Vec3f normal_to_cell;
    bool hit = false;
};

class Grid : public Object3D {
public:
    typedef std::vector<Object3D*> Cell;
private:
    static PhongMaterial GridMaterial;

    int _nx, _ny, _nz;
    Vec3f _bbMin, _bbMax;
    Vec3f _step;

    void _initializeRayMarch(MarchingInfo &mi, const Ray &r, float tmin);
    void _addEnteredCell(int i, int j, int k, int index, int positive);

    void _nextCell(MarchingInfo &info);
    PhongMaterial* _getCellMaterial(int index) const;

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
    PhongMaterial _materialSchema[SchemaColorNumber];

    std::vector<std::shared_ptr<Object3D>> _infiniteObjects;

public:
    bool visualizeGrid = false;

    std::vector<std::vector<std::vector<Cell>>> cells;

    Grid(const std::shared_ptr<BoundingBox> &bb, int nx, int ny, int nz);
    bool intersect(const Ray &r, Hit &h, float tmin) override;
    void paint() override;

    void getN(int &nx, int &ny, int &nz);
    Vec3f getBBMin() { return _bbMin; }
    Vec3f getBBMax() { return _bbMax; }
    Vec3f getStep() { return _step; }
    void getGridCellIndex(const Vec3f &p, int index[3]);

    void refreshColorSchema() { _crtColorIndex = 0; }

    void addInfiniteObject(const std::shared_ptr<Object3D> &object) { _infiniteObjects.emplace_back(object); }
};

#endif //ASSIGNMENTS_GRID_H
