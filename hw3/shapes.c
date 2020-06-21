#include "shapes.h"
#include "trig.h"
#include "structs.h"

// draws a vertex in polar coordinates
void Vertex(double th, double ph)
{
   glColor3f(Cos(th)*Cos(th) , Sin(ph)*Sin(ph) , Sin(th)*Sin(th));
   glVertex3d(Sin(th)*Cos(ph) , Sin(ph) , Cos(th)*Cos(ph));
}

void cube(double x, double y, double z, double d, double th)
{
  cuboid(x, y, z, d, d, d, th);
}

// draws a cuboid at (x,y,z), dimensions (dx,dy,dz), rotated th about the y axis
void cuboid(double x,double y,double z, double dx,double dy,double dz, double th)
{
   //  Save transformation
   glPushMatrix();
   //  Offset
   glTranslated(x,y,z);
   glRotated(th,0,1,0);
   glScaled(dx,dy,dz);
   //  Cube
   glBegin(GL_QUADS);
   //  Front
   glVertex3f(-1,-1, 1);
   glVertex3f(+1,-1, 1);
   glVertex3f(+1,+1, 1);
   glVertex3f(-1,+1, 1);
   //  Back
   glVertex3f(+1,-1,-1);
   glVertex3f(-1,-1,-1);
   glVertex3f(-1,+1,-1);
   glVertex3f(+1,+1,-1);
   //  Right
   glVertex3f(+1,-1,+1);
   glVertex3f(+1,-1,-1);
   glVertex3f(+1,+1,-1);
   glVertex3f(+1,+1,+1);
   //  Left
   glVertex3f(-1,-1,-1);
   glVertex3f(-1,-1,+1);
   glVertex3f(-1,+1,+1);
   glVertex3f(-1,+1,-1);
   //  Top
   glVertex3f(-1,+1,+1);
   glVertex3f(+1,+1,+1);
   glVertex3f(+1,+1,-1);
   glVertex3f(-1,+1,-1);
   //  Bottom
   glVertex3f(-1,-1,-1);
   glVertex3f(+1,-1,-1);
   glVertex3f(+1,-1,+1);
   glVertex3f(-1,-1,+1);
   //  End
   glEnd();
   //  Undo transformations
   glPopMatrix();
}

// draws a sphere at (x,y,z), radius (r)
void sphere1(double x,double y,double z,double r)
{
   const int d=15;
   int th,ph;

   //  Save transformation
   glPushMatrix();
   //  Offset and scale
   glTranslated(x,y,z);
   glScaled(r,r,r);

   //  South pole cap
   glBegin(GL_TRIANGLE_FAN);
   Vertex(0,-90);
   for (th=0;th<=360;th+=d)
   {
      Vertex(th,d-90);
   }
   glEnd();

   //  Latitude bands
   for (ph=d-90;ph<=90-2*d;ph+=d)
   {
      glBegin(GL_QUAD_STRIP);
      for (th=0;th<=360;th+=d)
      {
         Vertex(th,ph);
         Vertex(th,ph+d);
      }
      glEnd();
   }

   //  North pole cap
   glBegin(GL_TRIANGLE_FAN);
   Vertex(0,90);
   for (th=0;th<=360;th+=d)
   {
      Vertex(th,90-d);
   }
   glEnd();

   //  Undo transformations
   glPopMatrix();
}

// draws a sphere (version 2) at (x,y,z), radius (r)
void sphere2(double x,double y,double z,double r)
{
   const int d=5;
   int th,ph;

   //  Save transformation
   glPushMatrix();
   //  Offset and scale
   glTranslated(x,y,z);
   glScaled(r,r,r);

   //  Latitude bands
   for (ph=-90;ph<90;ph+=d)
   {
      glBegin(GL_QUAD_STRIP);
      for (th=0;th<=360;th+=d)
      {
         Vertex(th,ph);
         Vertex(th,ph+d);
      }
      glEnd();
   }

   //  Undo transformations
   glPopMatrix();
}

void xyplane(double x, double y, double z, double sx, double sy)
{
  //  Save transformation
  glPushMatrix();
  //  Offset
  glTranslated(sx,sy,z);
  glScaled(x,y,0);
  //  Cube
  glBegin(GL_QUADS);
  //  Front
  glVertex3f( 1, 1, z);
  glVertex3f( 1,-1, z);
  glVertex3f(-1,-1, z);
  glVertex3f(-1, 1, z);
  //  End
  glEnd();
  //  Undo transformations
  glPopMatrix();
}

void xzplane(double x, double y, double z, double sx, double sz)
{
  //  Save transformation
  glPushMatrix();
  //  Offset
  glTranslated(sx,y,sz);
  glScaled(x,0,z);
  //  Cube
  glBegin(GL_QUADS);
  //  Front
  glVertex3f( 1, y, 1);
  glVertex3f( 1, y,-1);
  glVertex3f(-1, y,-1);
  glVertex3f(-1, y, 1);
  //  End
  glEnd();
  //  Undo transformations
  glPopMatrix();
}

void yzplane(double x, double y, double z, double sy, double sz)
{
  //  Save transformation
  glPushMatrix();
  //  Offset
  glTranslated(x,sy,sz);
  glScaled(0,y,z);
  //  Cube
  glBegin(GL_QUADS);
  //  Front
  glVertex3f( x, 1, 1);
  glVertex3f( x, 1,-1);
  glVertex3f( x,-1,-1);
  glVertex3f( x,-1, 1);
  //  End
  glEnd();
  //  Undo transformations
  glPopMatrix();
}

void house(double x, double y, double z, double dx, double dy, double dz, double th, float r, float g, float b)
{
  float shiny = 1;
  float white[] = {1,1,1,1};
  float black[] = {0,0,0,1};
  glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,shiny);
  glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
  glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,black);

  //  Save transformation
  glPushMatrix();
  //  Offset
  glTranslated(x,y,z);
  glRotated(th,0,1,0);
  glScaled(dx,dy,dz);
  //  Cube
  glBegin(GL_QUADS);
  //  Front
  glColor3f(r + 0.1, g + 0.1, b + 0.1);
  //L
  glVertex3f(-1,-1, 1);
  glVertex3f(-0.3,-1, 1);
  glVertex3f(-0.3,+1, 1);
  glVertex3f(-1,+1, 1);
  //C
  glVertex3f(-0.3,0, 1);
  glVertex3f(0.3,0, 1);
  glVertex3f(.3,+1, 1);
  glVertex3f(-.3,+1, 1);
  //R
  glVertex3f(.3,-1, 1);
  glVertex3f(+1,-1, 1);
  glVertex3f(+1,+1, 1);
  glVertex3f(.3,+1, 1);
  //  Back
  glColor3f(r + 0.1, g + 0.1, b - 0.1);
  glVertex3f(+1,-1,-1);
  glVertex3f(-1,-1,-1);
  glVertex3f(-1,+1,-1);
  glVertex3f(+1,+1,-1);
  //  Right
  glColor3f(r + 0.1, g - 0.1, b + 0.1);
  glVertex3f(+1,-1,+1);
  glVertex3f(+1,-1,-1);
  glVertex3f(+1,+1,-1);
  glVertex3f(+1,+1,+1);
  //  Left
  glColor3f(r + 0.1, g - 0.1, b - 0.1);
  glVertex3f(-1,-1,-1);
  glVertex3f(-1,-1,+1);
  glVertex3f(-1,+1,+1);
  glVertex3f(-1,+1,-1);
  //  End
  glEnd();
  // Pyramid Roof
  glBegin(GL_TRIANGLES);
  glColor3f(r - 0.2, g - 0.2, b - 0.2);
  glVertex3d( 1.25, 1, 1.25);
  glVertex3d( 1.25, 1,-1.25);
  glVertex3d(0,1.5,0);

  glColor3f(r - 0.2, g - 0.2, b - 0.3);
  glVertex3d( 1.25, 1, 1.25);
  glVertex3d(-1.25, 1, 1.25);
  glVertex3d(0,1.5,0);

  glColor3f(r - 0.2, g - 0.3, b - 0.2);
  glVertex3d(-1.25, 1, 1.25);
  glVertex3d(-1.25, 1,-1.25);
  glVertex3d(0,1.5,0);

  glColor3f(r - 0.3, g - 0.2, b - 0.2);
  glVertex3d( 1.25, 1,-1.25);
  glVertex3d(-1.25, 1,-1.25);
  glVertex3d(0,1.5,0);

  glEnd();
  //  Undo transformations
  glPopMatrix();
}
