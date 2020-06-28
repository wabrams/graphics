#include "framework.h"

#ifndef SHAPES_H
#define SHAPES_H

enum cuboid_faces_en
{
    CF_FRONT = 0,
    CF_BACK = 1,
    CF_LEFT = 2,
    CF_RIGHT = 3,
    CF_TOP = 4,
    CF_BOTTOM = 5
};

typedef struct cuboid_st
{
  float x, y, z;
  float dimx, dimy, dimz;
  float th;
  color_t colors[6]; //TODO: make this pointer, reduces memory stored for repeated color references
} cuboid_t;

void drawCuboid(cuboid_t * c);

#endif // SHAPES_H
