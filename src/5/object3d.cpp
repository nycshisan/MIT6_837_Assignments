//
// Created by Nycshisan on 2018/7/30.
//

#include "object3d.h"

#include "grid.h"
#include "transform.h"
#include "boundingbox.h"

void Object3D::insertIntoGrid(Grid *g, Matrix *m) {
    if (m != nullptr && _bb != nullptr) {
        // only handles transformations && discards planes etc.
        auto transformedBB = Transform::TransformBoundingBox(*this, *m);
        int iMin, jMin, kMin, iMax, jMax, kMax;
        g->getGridCellIndex(transformedBB.getMin(), iMin, jMin, kMin);
        g->getGridCellIndex(transformedBB.getMax(), iMax, jMax, kMax);
        for (int i = iMin; i <= iMax; ++i) {
            for (int j = jMin; j <= jMax; ++j) {
                for (int k = kMin; k <= kMax; ++k) {
                    g->cells[i][j][k].emplace_back(this);
                }
            }
        }
    }
}
