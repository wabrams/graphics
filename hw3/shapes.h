#ifndef SHAPES_H
#define SHAPES_H

#define GL_GLEXT_PROTOTYPES
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include "structs.h"
// generic shapes
void Vertex(double th, double ph);
void cube(double x, double y, double z, double d, double th);
void cuboid(double x,double y,double z, double dx,double dy,double dz, double th);
void sphere1(double x,double y,double z,double r);
void sphere2(double x,double y,double z,double r);
void xyplane(double x, double y, double z, double sx, double sy);
void xzplane(double x, double y, double z, double sx, double sz, unsigned int t);
void yzplane(double x, double y, double z, double sy, double sz);
// special shapes
void house(double x, double y, double z, double dx, double dy, double dz, double th, float r, float g, float b, unsigned int t1, unsigned int t2);
void sun(double x, double y, double z, double r, double e, double s, int i);
#endif //SHAPES_H
