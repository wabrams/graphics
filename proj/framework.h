#ifndef FRAMEWORK_H
#define FRAMEWORK_H

#define Cos(x) (cos((x)*3.1415927/180))
#define Sin(x) (sin((x)*3.1415927/180))
#define Tan(x) (tan((x)*3.1415927/180))
#define UNUSED(x) (void)(x)

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

typedef enum cubefaces_en
{
  CUBE_FACE_TOP,
  CUBE_FACE_BOTTOM,
  CUBE_FACE_FRONT,
  CUBE_FACE_BACK,
  CUBE_FACE_LEFT,
  CUBE_FACE_RIGHT,
} cubefaces_t;

//handled in framework.c
void check(const char * where);
void fatal(const char * format, ...);
void reverseBytes(void *, const int n);
unsigned int loadTextureFromBMP(const char * file);

#endif // FRAMEWORK_H
