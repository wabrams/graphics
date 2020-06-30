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

typedef struct vertex_st
{
  float x, y, z;
} vertex_t;

typedef struct grid_st
{
  int x, y, z;
} grid_t;

typedef struct car_st
{
  float x, y, z;
  float dimx, dimy, dimz;
  float th;
  float vel, acc;
} car_t;

typedef struct light_s
{
  int  emission;      // Emission intensity (%)
  int  ambient;       // Ambient intensity  (%)
  int  diffuse;       // Diffuse intensity  (%)
  int  specular;      // Specular intensity (%)
  int  shininess;     // Shininess (power of two)
  float shiny;        // Shininess (value)
} light_t;

//handled in framework.c
void check(const char * where);
void fatal(const char * format, ...);

void reverseBytes(void *, const int n);
unsigned int loadTextureFromBMP(const char * file);

void getCurrentGrid(vertex_t * v, grid_t * g);

#endif // FRAMEWORK_H
