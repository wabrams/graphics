#ifndef CAMERA_H
#define CAMERA_H

#include <stdbool.h>

struct window_s
{
  int w;
  int h;
};

struct coord1_s
{
  double x;
};

struct coord2_s
{
  double x, y;
};

struct coord3_s
{
  double x, y, z;
};

struct ccoord_s
{
  struct coord3_s c;
  float pitch, yaw;
};

struct view_s
{
  int th;
  int ph;
  float dim;
  bool axes;
  int fov;
  float asp;
};

struct color_s
{
  int r;
  int g;
  int b;
};

typedef struct window_s window_t;
typedef struct coord1_s coord1_t;
typedef struct coord2_s coord2_t;
typedef struct coord3_s coord3_t;
typedef struct ccoord_s ccoord_t;
typedef struct color_s color_t;
typedef struct view_s view_t;
typedef struct pos_s pos_t;

#endif
