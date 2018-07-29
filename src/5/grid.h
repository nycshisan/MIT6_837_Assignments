//
// Created by Nycshisan on 2018/7/29.
//

#ifndef ASSIGNMENTS_GRID_H
#define ASSIGNMENTS_GRID_H

#include "object3d.h"
#include "boundingbox.h"


class Grid : public Object3D {
    BoundingBox *_boundingBox;

public:
    BoundingBox *getBoundingBox() { return _boundingBox; }
};

#endif //ASSIGNMENTS_GRID_H
