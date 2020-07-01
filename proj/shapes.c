#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>

#define GL_GLEXT_PROTOTYPES
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include "shapes.h"
#include "framework.h"

#define NUM_TEXTURES 6
const char * texture_filepaths[] = {"img/wall.bmp", "img/roof.bmp", "img/grass.bmp", "img/sky0.bmp", "img/sky1.bmp", "img/road.bmp"};
unsigned int textures[NUM_TEXTURES];

void loadTextures()
{
  for (int i = 0; i < NUM_TEXTURES; i++)
    textures[i] = loadTextureFromBMP(texture_filepaths[i]);
}

void drawCuboid(cuboid_t * c)
{
  glPushMatrix();
  // offset
  glTranslatef(c -> x, c -> y, c -> z);
  glRotatef(c -> th, 0, 1, 0);
  glScalef(c -> dimx, c -> dimy, c -> dimz);
  // draw
  glBegin(GL_QUADS);
  // front
  glNormal3d(0, 0, 1);
  glColor3ub(c->colors[CF_FRONT].r, c->colors[CF_FRONT].g, c->colors[CF_FRONT].b);
  glVertex3f(-1,-1, 1);
  glVertex3f(+1,-1, 1);
  glVertex3f(+1,+1, 1);
  glVertex3f(-1,+1, 1);
  // back
  glNormal3d(0, 0, -1);
  glColor3ub(c->colors[CF_BACK].r, c->colors[CF_BACK].g, c->colors[CF_BACK].b);
  glVertex3f(+1,-1,-1);
  glVertex3f(-1,-1,-1);
  glVertex3f(-1,+1,-1);
  glVertex3f(+1,+1,-1);
  // left
  glNormal3d(-1, 0, 0);
  glColor3ub(c->colors[CF_LEFT].r, c->colors[CF_LEFT].g, c->colors[CF_LEFT].b);
  glVertex3f(-1,-1,-1);
  glVertex3f(-1,-1,+1);
  glVertex3f(-1,+1,+1);
  glVertex3f(-1,+1,-1);
  // right
  glNormal3d(1, 0, 0);
  glColor3ub(c->colors[CF_RIGHT].r, c->colors[CF_RIGHT].g, c->colors[CF_RIGHT].b);
  glVertex3f(+1,-1,+1);
  glVertex3f(+1,-1,-1);
  glVertex3f(+1,+1,-1);
  glVertex3f(+1,+1,+1);
  // top
  glNormal3d(0, 1, 0);
  glColor3ub(c->colors[CF_TOP].r, c->colors[CF_TOP].g, c->colors[CF_TOP].b);
  glVertex3f(-1,+1,+1);
  glVertex3f(+1,+1,+1);
  glVertex3f(+1,+1,-1);
  glVertex3f(-1,+1,-1);
  // bottom
  glNormal3d(0, -1, 0);
  glColor3ub(c->colors[CF_BOTTOM].r, c->colors[CF_BOTTOM].g, c->colors[CF_BOTTOM].b);
  glVertex3f(-1,-1,-1);
  glVertex3f(+1,-1,-1);
  glVertex3f(+1,-1,+1);
  glVertex3f(-1,-1,+1);
  // end
  glEnd();
  glPopMatrix();
}

void drawSkybox(vertex_t * v, float D, color_t * c)
{
  glPushMatrix();
  glTranslated(v -> x, v -> y, v -> z);
  // glEnable(GL_TEXTURE_2D);
  glColor3ub(c -> r, c -> g, c -> b);
  //  Sides
  glBindTexture(GL_TEXTURE_2D,textures[3]);
  glBegin(GL_QUADS);
    /* glTexCoord2f(0.00,0); */ glVertex3f(-D,-D,-D);
    /* glTexCoord2f(0.25,0); */ glVertex3f(+D,-D,-D);
    /* glTexCoord2f(0.25,1); */ glVertex3f(+D,+D,-D);
    /* glTexCoord2f(0.00,1); */ glVertex3f(-D,+D,-D);

    /* glTexCoord2f(0.25,0); */ glVertex3f(+D,-D,-D);
    /* glTexCoord2f(0.50,0); */ glVertex3f(+D,-D,+D);
    /* glTexCoord2f(0.50,1); */ glVertex3f(+D,+D,+D);
    /* glTexCoord2f(0.25,1); */ glVertex3f(+D,+D,-D);

    /* glTexCoord2f(0.50,0); */ glVertex3f(+D,-D,+D);
    /* glTexCoord2f(0.75,0); */ glVertex3f(-D,-D,+D);
    /* glTexCoord2f(0.75,1); */ glVertex3f(-D,+D,+D);
    /* glTexCoord2f(0.50,1); */ glVertex3f(+D,+D,+D);

    /* glTexCoord2f(0.75,0); */ glVertex3f(-D,-D,+D);
    /* glTexCoord2f(1.00,0); */ glVertex3f(-D,-D,-D);
    /* glTexCoord2f(1.00,1); */ glVertex3f(-D,+D,-D);
    /* glTexCoord2f(0.75,1); */ glVertex3f(-D,+D,+D);
  // glEnd();
  //  Top and bottom
  // glBindTexture(GL_TEXTURE_2D,textures[4]);
  // glBegin(GL_QUADS);
  /* glTexCoord2f(0.0,0); */ glVertex3f(+D,+D,-D);
  /* glTexCoord2f(0.5,0); */ glVertex3f(+D,+D,+D);
  /* glTexCoord2f(0.5,1); */ glVertex3f(-D,+D,+D);
  /* glTexCoord2f(0.0,1); */ glVertex3f(-D,+D,-D);

  /* glTexCoord2f(1.0,1); */ glVertex3f(-D,-D,+D);
  /* glTexCoord2f(0.5,1); */ glVertex3f(+D,-D,+D);
  /* glTexCoord2f(0.5,0); */ glVertex3f(+D,-D,-D);
  /* glTexCoord2f(1.0,0); */ glVertex3f(-D,-D,-D);

  glEnd();
  glDisable(GL_TEXTURE_2D);
  glPopMatrix();
}

void drawCar(car_t * c)
{
  glPushMatrix();
  // offset
  glTranslatef(c -> x, c -> y, c -> z);
  glRotatef(c -> th, 0, 1, 0);
  glScalef(c -> dimx / 2.0, c -> dimy, c -> dimz / 2.0);
  // frame
  float shiny = 1;
  float white[] = {1,1,1,1};
  float black[] = {0,0,0,1};
  glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,shiny);
  glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
  glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,black);
  glBegin(GL_QUADS);
  glColor3ub(0xFF, 0x00, 0x00);
    // bottom
    glNormal3d(0, -1, 0);
    glVertex3f(-1.0, 0.2,-1.0);
    glVertex3f(-1.0, 0.2, 1.0);
    glVertex3f( 1.0, 0.2, 1.0);
    glVertex3f( 1.0, 0.2,-1.0);
    // top
    glNormal3d(0, +1, 0);
    glVertex3f(-1.0, 0.5,-1.0);
    glVertex3f(-1.0, 0.5, 1.0);
    glVertex3f( 1.0, 0.5, 1.0);
    glVertex3f( 1.0, 0.5,-1.0);
    // front bumper
    glNormal3d(0, 0, -1);
    glVertex3f(-1.0, 0.2,-1.0);
    glVertex3f( 1.0, 0.2,-1.0);
    glVertex3f( 1.0, 0.5,-1.0);
    glVertex3f(-1.0, 0.5,-1.0);
    // rear bumper
    glNormal3d(0, 0, +1);
    glVertex3f(-1.0, 0.2, 1.0);
    glVertex3f( 1.0, 0.2, 1.0);
    glVertex3f( 1.0, 0.5, 1.0);
    glVertex3f(-1.0, 0.5, 1.0);
    // left side
    glNormal3d(-1, 0, 0);
    glVertex3f(-1.0, 0.2,-1.0);
    glVertex3f(-1.0, 0.2, 1.0);
    glVertex3f(-1.0, 0.5, 1.0);
    glVertex3f(-1.0, 0.5,-1.0);
    // right side
    glNormal3d(+1, 0, 0);
    glVertex3f( 1.0, 0.2,-1.0);
    glVertex3f( 1.0, 0.2, 1.0);
    glVertex3f( 1.0, 0.5, 1.0);
    glVertex3f( 1.0, 0.5,-1.0);
    // roof top
    glNormal3d(0, +1, 0);
    glVertex3f(-1.0, 0.7,-0.1);
    glVertex3f(-1.0, 0.7, 0.6);
    glVertex3f( 1.0, 0.7, 0.6);
    glVertex3f( 1.0, 0.7,-0.1);
    // roof left side
    glNormal3d(-1,  0,  0);
    glVertex3f(-1.0,  0.5, -0.1);
    glVertex3f(-1.0,  0.5,  0.6);
    glVertex3f(-1.0,  0.7,  0.6);
    glVertex3f(-1.0,  0.7, -0.1);
    // roof right side
    glNormal3d( 1,  0,  0);
    glVertex3f( 1.0,  0.5, -0.1);
    glVertex3f( 1.0,  0.5,  0.6);
    glVertex3f( 1.0,  0.7,  0.6);
    glVertex3f( 1.0,  0.7, -0.1);
  glColor3ub(0x00, 0xFF, 0xFF);
    // front windshield
    glNormal3f(0,3,-2);
    glVertex3f(-1.0,  0.5, -0.4);
    glVertex3f(-1.0,  0.7, -0.1);
    glVertex3f( 1.0,  0.7, -0.1);
    glVertex3f( 1.0,  0.5, -0.4);
    // rear windshield
    glNormal3f(0,1,2);
    glVertex3f(-1.0,  0.5,  0.7);
    glVertex3f(-1.0,  0.7,  0.6);
    glVertex3f( 1.0,  0.7,  0.6);
    glVertex3f( 1.0,  0.5,  0.7);
  glEnd();
  glBegin(GL_TRIANGLES);
    //front left
    glNormal3d(-1, 0, 0);
    glVertex3f(-1.0,  0.7, -0.1);
    glVertex3f(-1.0,  0.5, -0.1);
    glVertex3f(-1.0,  0.5, -0.4);
    //back left
    glVertex3f(-1.0,  0.7,  0.6);
    glVertex3f(-1.0,  0.5,  0.6);
    glVertex3f(-1.0,  0.5,  0.7);
    //back right
    glNormal3d(+1, 0, 0);
    glVertex3f( 1.0,  0.7,  0.6);
    glVertex3f( 1.0,  0.5,  0.6);
    glVertex3f( 1.0,  0.5,  0.7);
    //front right
    glVertex3f( 1.0,  0.7, -0.1);
    glVertex3f( 1.0,  0.5, -0.1);
    glVertex3f( 1.0,  0.5, -0.4);
  glEnd();
  //headlights (casing):
  glColor3ub(0xAA, 0xAA, 0xAA); //light gray
  glBegin(GL_QUADS);
    glNormal3d(-1, 0, 0);
      //left headlight, left face
      glVertex3f(-0.6,  0.45, -1.1);
      glVertex3f(-0.6,  0.35, -1.1);
      glVertex3f(-0.6,  0.35, -1.0);
      glVertex3f(-0.6,  0.45, -1.0);
      //right headlight, left face
      glVertex3f( 0.4,  0.45, -1.1);
      glVertex3f( 0.4,  0.35, -1.1);
      glVertex3f( 0.4,  0.35, -1.0);
      glVertex3f( 0.4,  0.45, -1.0);
    glNormal3d(0, -1, 0);
      //left headlight, bottom face
      glVertex3f( 0.0,  0.35,  0.0);
      glVertex3f( 0.0,  0.35,  0.0);
      glVertex3f( 0.0,  0.35,  0.0);
      glVertex3f( 0.0,  0.35,  0.0);
      //right headlight, bottom face
      glVertex3f( 0.0,  0.35,  0.0);
      glVertex3f( 0.0,  0.35,  0.0);
      glVertex3f( 0.0,  0.35,  0.0);
      glVertex3f( 0.0,  0.35,  0.0);
    glNormal3d(+1, 0, 0);
    //left headlight, right face
      glVertex3f(-0.4,  0.45, -1.1);
      glVertex3f(-0.4,  0.35, -1.1);
      glVertex3f(-0.4,  0.35, -1.0);
      glVertex3f(-0.4,  0.45, -1.0);
      //right headlight, right face
      glVertex3f( 0.6,  0.45, -1.1);
      glVertex3f( 0.6,  0.35, -1.1);
      glVertex3f( 0.6,  0.35, -1.0);
      glVertex3f( 0.6,  0.45, -1.0);
    glNormal3d(0, +1, 0);
    //left headlight, top face
      glVertex3f( 0.0,  0.45,  0.0);
      glVertex3f( 0.0,  0.45,  0.0);
      glVertex3f( 0.0,  0.45,  0.0);
      glVertex3f( 0.0,  0.45,  0.0);
      //right headlight, top face
      glVertex3f( 0.0,  0.45,  0.0);
      glVertex3f( 0.0,  0.45,  0.0);
      glVertex3f( 0.0,  0.45,  0.0);
      glVertex3f( 0.0,  0.45,  0.0);
  glEnd();
  //headlights (lights):
  glColor3ub(0xFF, 0xFF, 0x00);
  float yellow[] = {1.0,1.0,0.0,1.0};
  float Emission[]  = {0.0,0.0,0.00,1.0};
  glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,shiny);
  glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,yellow);
  glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,Emission);
  glBegin(GL_QUADS);
    glNormal3d(0, 0, -1);
    //right
    glVertex3f( 0.4, 0.35,-1.1);
    glVertex3f( 0.6, 0.35,-1.1);
    glVertex3f( 0.6, 0.45,-1.1);
    glVertex3f( 0.4, 0.45,-1.1);
    //left
    glVertex3f(-0.4, 0.35,-1.1);
    glVertex3f(-0.6, 0.35,-1.1);
    glVertex3f(-0.6, 0.45,-1.1);
    glVertex3f(-0.4, 0.45,-1.1);
  glEnd();
  glPopMatrix();
}

static void polarVertex(float th, float ph)
{
   glVertex3d(Sin(th)*Cos(ph) , Sin(ph) , Cos(th)*Cos(ph));
}

void drawSun(vertex_t * v, float r, float emission, float shiny, int inc)
{
   int th,ph;
   float yellow[] = {1.0,1.0,0.0,1.0};
   float Emission[]  = {0.0,0.0,(float)0.1*emission,1.0};
   //  Save transformation
   glPushMatrix();
   //  Offset, scale and rotate
   glTranslated(v -> x, v -> y,v -> z);
   glScaled(r,r,r);
   //  yellow sphere
   glColor3f(1,1,0);
   glMaterialf(GL_FRONT,GL_SHININESS,shiny);
   glMaterialfv(GL_FRONT,GL_SPECULAR,yellow);
   glMaterialfv(GL_FRONT,GL_EMISSION,Emission);
   //  Bands of latitude
   for (ph=-90;ph<90;ph+=inc)
   {
      glBegin(GL_QUAD_STRIP);
      for (th=0;th<=360;th+=2*inc)
      {
         polarVertex(th,ph);
         polarVertex(th,ph+inc);
      }
      glEnd();
   }
   //  Undo transofrmations
   glPopMatrix();
}

static void varyDimensions(vertex_t * v)
{
  v -> x = 0.2 + (((rand() % 10) - 10) / 100.0);
  v -> y = 0.2 + (((rand() % 10) - 10) / 100.0);
  v -> z = 0.2 + (((rand() % 10) - 10) / 100.0);
}

static void drawHouse()
{
  float shiny = 1;
  float white[] = {1,1,1,1};
  float black[] = {0,0,0,1};
  glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,shiny);
  glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
  glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,black);
  glEnable(GL_TEXTURE_2D);
  glTexEnvi(GL_TEXTURE_ENV , GL_TEXTURE_ENV_MODE , GL_MODULATE);
  glBindTexture(GL_TEXTURE_2D, textures[0]);
  glBegin(GL_QUADS);
    // left wall
    glNormal3d(-1, 0, 0);
    glTexCoord2f(0.0,0.0); glVertex3f(0.1, 0.0, 0.1);
    glTexCoord2f(1.0,0.0); glVertex3f(0.1, 0.0, 0.9);
    glTexCoord2f(1.0,1.0); glVertex3f(0.1, 0.8, 0.9);
    glTexCoord2f(0.0,1.0); glVertex3f(0.1, 0.8, 0.1);
    // front wall
    glNormal3d(0, 0, +1);
    glTexCoord2f(0.0, 0.0); glVertex3f(0.1, 0.0, 0.9);
    glTexCoord2f(1.0, 0.0); glVertex3f(0.9, 0.0, 0.9);
    glTexCoord2f(1.0, 1.0); glVertex3f(0.9, 0.8, 0.9);
    glTexCoord2f(0.0, 1.0); glVertex3f(0.1, 0.8, 0.9);
    // right wall
    glNormal3d(+1, 0, 0);
    glTexCoord2f(0.0, 0.0); glVertex3f(0.9, 0.0, 0.9);
    glTexCoord2f(1.0, 0.0); glVertex3f(0.9, 0.0, 0.1);
    glTexCoord2f(1.0, 1.0); glVertex3f(0.9, 0.8, 0.1);
    glTexCoord2f(0.0, 1.0); glVertex3f(0.9, 0.8, 0.9);
    // back wall
    glNormal3d(0, 0, -1);
    glTexCoord2f(0.0, 0.0); glVertex3f(0.9, 0.0, 0.1);
    glTexCoord2f(1.0, 0.0); glVertex3f(0.1, 0.0, 0.1);
    glTexCoord2f(1.0, 1.0); glVertex3f(0.1, 0.8, 0.1);
    glTexCoord2f(0.0, 1.0); glVertex3f(0.9, 0.8, 0.1);
    // end
  glEnd();

  glBindTexture(GL_TEXTURE_2D, textures[1]);
  glColor3ub(0x77, 0x77 , 0x77); //TODO: could this be a triangle strip?
  glBegin(GL_TRIANGLES);
    // left roof
    glNormal3f(-0.2,  0.5,  0.0);
    glTexCoord2f(0.0, 0.0); glVertex3d(0.0, 0.8, 0.0);
    glTexCoord2f(1.0, 0.0); glVertex3d(0.0, 0.8, 1.0);
    glTexCoord2f(0.5, 1.0); glVertex3d(0.5, 1.0, 0.5);
    // front roof
    glNormal3f( 0.0, 0.5, +0.2);
    glTexCoord2f(0.0, 0.0); glVertex3d(0.0, 0.8, 1.0);
    glTexCoord2f(1.0, 0.0); glVertex3d(1.0, 0.8, 1.0);
    glTexCoord2f(0.5, 1.0); glVertex3d(0.5, 1.0, 0.5);
    // right roof
    glNormal3f( 0.2,  0.5,  0.0);
    glTexCoord2f(0.0, 0.0); glVertex3d(1.0, 0.8, 1.0);
    glTexCoord2f(1.0, 0.0); glVertex3d(1.0, 0.8, 0.0);
    glTexCoord2f(0.5, 1.0); glVertex3d(0.5, 1.0, 0.5);
    // back roof
    glNormal3f( 0.0,  0.5, -0.2);
    glTexCoord2f(0.0, 0.0); glVertex3d(1.0, 0.8, 0.0);
    glTexCoord2f(1.0, 0.0); glVertex3d(0.0, 0.8, 0.0);
    glTexCoord2f(0.5, 1.0); glVertex3d(0.5, 1.0, 0.5);

  glEnd();
  glDisable(GL_TEXTURE_2D);
}

void drawGrid(grid_t * g)
{
  srand((g -> x << 16) + (g -> z));

  // get to the current grid
  glPushMatrix();
  glTranslated(g -> x, g -> y, g -> z);

  // draw the floor
  float shiny = 1;
  float white[] = {1,1,1,1};
  float black[] = {0,0,0,1};
  glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,shiny);
  glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
  glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,black);
  glEnable(GL_TEXTURE_2D);
  glTexEnvi(GL_TEXTURE_ENV , GL_TEXTURE_ENV_MODE , GL_MODULATE);
  glBindTexture(GL_TEXTURE_2D, textures[5]);
  glBegin(GL_QUADS);
  // white (intersections)
  glNormal3d( 0,  1,  0);
  glColor3ub(0xAA, 0xAA, 0xAA);
    //top-left
    glTexCoord2f(0.0, 0.5); glVertex3f(0.0, 0.0, 0.0);
    glTexCoord2f(0.0, 0.0); glVertex3f(0.0, 0.0, 0.1);
    glTexCoord2f(0.5, 0.0); glVertex3f(0.1, 0.0, 0.1);
    glTexCoord2f(0.5, 0.5); glVertex3f(0.1, 0.0, 0.0);
    //bottom-left
    glTexCoord2f(0.5, 0.0); glVertex3f(0.0, 0.0, 0.9);
    glTexCoord2f(0.5, 0.5); glVertex3f(0.0, 0.0, 1.0);
    glTexCoord2f(0.0, 0.5); glVertex3f(0.1, 0.0, 1.0);
    glTexCoord2f(0.0, 0.0); glVertex3f(0.1, 0.0, 0.9);
    //bottom-right
    glTexCoord2f(0.5, 0.0); glVertex3f(0.9, 0.0, 0.9);
    glTexCoord2f(0.0, 0.0); glVertex3f(0.9, 0.0, 1.0);
    glTexCoord2f(0.0, 0.5); glVertex3f(1.0, 0.0, 1.0);
    glTexCoord2f(0.5, 0.5); glVertex3f(1.0, 0.0, 0.9);
    //top-right
    glTexCoord2f(0.0, 0.0); glVertex3f(0.9, 0.0, 0.0);
    glTexCoord2f(0.5, 0.0); glVertex3f(0.9, 0.0, 0.1);
    glTexCoord2f(0.5, 0.5); glVertex3f(1.0, 0.0, 0.1);
    glTexCoord2f(0.0, 0.5); glVertex3f(1.0, 0.0, 0.0);
  glEnd();
  // gray (roads)
  glColor3ub(0xAA, 0xAA, 0xAA);
  glBegin(GL_QUADS);
    //up
    glTexCoord2f(0.5, 1.0); glVertex3f(0.1, 0.0, 0.0);
    glTexCoord2f(0.5, 0.0); glVertex3f(0.1, 0.0, 0.1);
    glTexCoord2f(1.0, 0.0); glVertex3f(0.9, 0.0, 0.1);
    glTexCoord2f(1.0, 1.0); glVertex3f(0.9, 0.0, 0.0);
    //left
    glTexCoord2f(1.0, 1.0); glVertex3f(0.0, 0.0, 0.1);
    glTexCoord2f(0.5, 1.0); glVertex3f(0.0, 0.0, 0.9);
    glTexCoord2f(0.5, 0.0); glVertex3f(0.1, 0.0, 0.9);
    glTexCoord2f(1.0, 0.0); glVertex3f(0.1, 0.0, 0.1);
    //down
    glTexCoord2f(1.0, 0.0); glVertex3f(0.1, 0.0, 0.9);
    glTexCoord2f(1.0, 1.0); glVertex3f(0.1, 0.0, 1.0);
    glTexCoord2f(0.5, 1.0); glVertex3f(0.9, 0.0, 1.0);
    glTexCoord2f(0.5, 0.0); glVertex3f(0.9, 0.0, 0.9);
    //right
    glTexCoord2f(0.5, 0.0); glVertex3f(0.9, 0.0, 0.1);
    glTexCoord2f(1.0, 0.0); glVertex3f(0.9, 0.0, 0.9);
    glTexCoord2f(1.0, 1.0); glVertex3f(1.0, 0.0, 0.9);
    glTexCoord2f(0.5, 1.0); glVertex3f(1.0, 0.0, 0.1);
  glEnd();
  // green (grass)
  glBindTexture(GL_TEXTURE_2D, textures[2]);
  glBegin(GL_QUADS);
  glColor3ub(0x00, 0x99, 0x00);
    glTexCoord2f(0.0, 0.0); glVertex3f(0.1, 0.0, 0.1);
    glTexCoord2f(4.0, 0.0); glVertex3f(0.1, 0.0, 0.9);
    glTexCoord2f(4.0, 4.0); glVertex3f(0.9, 0.0, 0.9);
    glTexCoord2f(0.0, 4.0); glVertex3f(0.9, 0.0, 0.1); //exceed 1 w/ texcoord2f forces tiling
  glEnd();
  glDisable(GL_TEXTURE_2D);

  //generate the house per this grid
  color_t hcolor;
  vertex_t rsize;
  unsigned int hsel = rand() % 16; //15 is 0b1111
  // NW house
  if (hsel & 0b0001)
  {
    varyDimensions(&rsize);
    glTranslated(0.1 + rsize.x, 0.0, 0.1 + rsize.z);
      hcolor.r = rand();
      hcolor.g = rand();
      hcolor.b = rand();
      glScalef(rsize.x, rsize.y, rsize.z);
      glColor3ub(hcolor.r, hcolor.g, hcolor.b);
      drawHouse();
      glScalef(1.0/rsize.x, 1.0/rsize.y, 1.0/rsize.z);
      glTranslated(-0.1 - rsize.x, 0.0, -0.1 - rsize.z);
  }
  // SW house
  if (hsel & 0b0010)
  {
    varyDimensions(&rsize);
    glTranslated(0.1 + rsize.x, 0.0, 0.5 + rsize.z);
      hcolor.r = rand();
      hcolor.g = rand();
      hcolor.b = rand();
      glScalef(rsize.x, rsize.y, rsize.z);
      glColor3ub(hcolor.r, hcolor.g, hcolor.b);
      drawHouse();
      glScalef(1.0/rsize.x, 1.0/rsize.y, 1.0/rsize.z);
      glTranslated(-0.1 - rsize.x, 0.0, -0.5 - rsize.z);
  }
  // SE house
  if (hsel & 0b0100)
  {
    varyDimensions(&rsize);
    glTranslated(0.5 + rsize.x, 0.0, 0.5 + rsize.z);
      hcolor.r = rand();
      hcolor.g = rand();
      hcolor.b = rand();
      glScalef(rsize.x, rsize.y, rsize.z);
      glColor3ub(hcolor.r, hcolor.g, hcolor.b);
      drawHouse();
      glScalef(1.0/rsize.x, 1.0/rsize.y, 1.0/rsize.z);
      glTranslated(-0.5 - rsize.x, 0.0, -0.5 - rsize.z);
  }
  // NE house
  if (hsel & 0b1000)
  {
    varyDimensions(&rsize);
    glTranslated(0.5 + rsize.x, 0.0, 0.1 + rsize.z);
      hcolor.r = rand();
      hcolor.g = rand();
      hcolor.b = rand();
      glScalef(rsize.x, rsize.y, rsize.z);
      glColor3ub(hcolor.r, hcolor.g, hcolor.b);
      drawHouse();
      glScalef(1.0/rsize.x, 1.0/rsize.y, 1.0/rsize.z);
      glTranslated(-0.5 - rsize.x, 0.0, -0.1 - rsize.z);
  }
  glPopMatrix();
}
