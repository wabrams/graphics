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

enum house_surfaces_en
{
  HS_WALLS = 0,
  HS_ROOF = 1,
  HS_DOOR = 2
};

typedef struct cuboid_st
{
  float x, y, z;
  float dimx, dimy, dimz;
  float th;
  color_t colors[6]; //TODO: make this pointer, reduces memory stored for repeated color references
} cuboid_t;

typedef struct house_st
{
  float x, y, z;
  float dimx, dimy, dimz;
  float th;
  color_t colors[3];
} house_t;

typedef struct floor_st
{
  float dimx, dimy, dimz;
  unsigned int tID;
  color_t color;
} floor_t;

void loadTextures();
void drawSkybox(vertex_t * v, float d);
void drawCuboid(cuboid_t * c);
void drawGrid(grid_t * g);

#endif // SHAPES_H
