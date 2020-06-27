#ifndef FRAMEWORK_H
#define FRAMEWORK_H

typedef struct window_st
{
  int width;
  int height;
  float asp;
} window_t;

typedef struct view_st
{
  int th;
  int ph;
  float dim;
  int fov;
} view_t;

typedef struct color_st
{
  unsigned int r: 8;
  unsigned int g: 8;
  unsigned int b: 8;
} color_t;

typedef struct player_st
{
  float x, y, z;
  float pitch, yaw;
  float speed;
} player_t;

typedef struct vertex_st
{
  float x, y, z;
} vertex_t;

#endif // FRAMEWORK_H
