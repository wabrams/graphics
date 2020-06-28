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

#define NUM_TEXTURES 2
const char * texture_filepaths[] = {"wall.bmp", "roof.bmp", "road.bmp"};
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
  glTranslatef(c -> x, c -> y, c -> z); //TODO: translatef vs rotatef
  glRotatef(c -> th, 0, 1, 0);
  glScalef(c -> dimx, c -> dimy, c -> dimz);
  // draw
  glBegin(GL_QUADS);
  // front
  glColor3ub(c->colors[CF_FRONT].r, c->colors[CF_FRONT].g, c->colors[CF_FRONT].b);
  glVertex3f(-1,-1, 1);
  glVertex3f(+1,-1, 1);
  glVertex3f(+1,+1, 1);
  glVertex3f(-1,+1, 1);
  // back
  glColor3ub(c->colors[CF_BACK].r, c->colors[CF_BACK].g, c->colors[CF_BACK].b);
  glVertex3f(+1,-1,-1);
  glVertex3f(-1,-1,-1);
  glVertex3f(-1,+1,-1);
  glVertex3f(+1,+1,-1);
  // left
  glColor3ub(c->colors[CF_LEFT].r, c->colors[CF_LEFT].g, c->colors[CF_LEFT].b);
  glVertex3f(-1,-1,-1);
  glVertex3f(-1,-1,+1);
  glVertex3f(-1,+1,+1);
  glVertex3f(-1,+1,-1);
  // right
  glColor3ub(c->colors[CF_RIGHT].r, c->colors[CF_RIGHT].g, c->colors[CF_RIGHT].b);
  glVertex3f(+1,-1,+1);
  glVertex3f(+1,-1,-1);
  glVertex3f(+1,+1,-1);
  glVertex3f(+1,+1,+1);
  // top
  glColor3ub(c->colors[CF_TOP].r, c->colors[CF_TOP].g, c->colors[CF_TOP].b);
  glVertex3f(-1,+1,+1);
  glVertex3f(+1,+1,+1);
  glVertex3f(+1,+1,-1);
  glVertex3f(-1,+1,-1);
  // bottom
  glColor3ub(c->colors[CF_BOTTOM].r, c->colors[CF_BOTTOM].g, c->colors[CF_BOTTOM].b);
  glVertex3f(-1,-1,-1);
  glVertex3f(+1,-1,-1);
  glVertex3f(+1,-1,+1);
  glVertex3f(-1,-1,+1);
  // end
  glEnd();
  glPopMatrix();
}

void drawGrid(grid_t * g)
{
  srand((g -> x << 16) + (g -> z));

  // get to the current grid
  glPushMatrix();
  glTranslated(g -> x, g -> y, g -> z);

  // draw the floor
  glBegin(GL_QUADS);
  // white (intersections)
  glColor3ub(0xFF, 0xFF, 0xFF);
    //top-left
    glVertex3f(0.0, 0.0, 0.0);
    glVertex3f(0.0, 0.0, 0.1);
    glVertex3f(0.1, 0.0, 0.1);
    glVertex3f(0.1, 0.0, 0.0);
    //bottom-left
    glVertex3f(0.0, 0.0, 0.9);
    glVertex3f(0.0, 0.0, 1.0);
    glVertex3f(0.1, 0.0, 1.0);
    glVertex3f(0.1, 0.0, 0.9);
    //bottom-right
    glVertex3f(0.9, 0.0, 0.9);
    glVertex3f(0.9, 0.0, 1.0);
    glVertex3f(1.0, 0.0, 1.0);
    glVertex3f(1.0, 0.0, 0.9);
    //top-right
    glVertex3f(0.9, 0.0, 0.0);
    glVertex3f(0.9, 0.0, 0.1);
    glVertex3f(1.0, 0.0, 0.1);
    glVertex3f(1.0, 0.0, 0.0);
  // gray (roads)
  glColor3ub(0xAA, 0xAA, 0xAA);
    //up
    glVertex3f(0.1, 0.0, 0.0);
    glVertex3f(0.1, 0.0, 0.1);
    glVertex3f(0.9, 0.0, 0.1);
    glVertex3f(0.9, 0.0, 0.0);
    //left
    glVertex3f(0.0, 0.0, 0.1);
    glVertex3f(0.0, 0.0, 0.9);
    glVertex3f(0.1, 0.0, 0.9);
    glVertex3f(0.1, 0.0, 0.1);
    //down
    glVertex3f(0.1, 0.0, 0.9);
    glVertex3f(0.1, 0.0, 1.0);
    glVertex3f(0.9, 0.0, 1.0);
    glVertex3f(0.9, 0.0, 0.9);
    //right
    glVertex3f(0.9, 0.0, 0.1);
    glVertex3f(0.9, 0.0, 0.9);
    glVertex3f(1.0, 0.0, 0.9);
    glVertex3f(1.0, 0.0, 0.1);
  // green (grass)
  glColor3ub(0x00, 0xAA, 0x00);
    glVertex3f(0.1, 0.0, 0.1);
    glVertex3f(0.1, 0.0, 0.9);
    glVertex3f(0.9, 0.0, 0.9);
    glVertex3f(0.9, 0.0, 0.1);
  glEnd();

  //generate the house per this grid
  color_t hcolor;
  hcolor.r = rand();
  hcolor.g = rand();
  hcolor.b = rand();
  glColor3ub(hcolor.r, hcolor.g, hcolor.b);

  glTranslated(0.5, 0.0, 0.5);
  glScalef(0.2, 0.2, 0.2);
  glEnable(GL_TEXTURE_2D);
  glTexEnvi(GL_TEXTURE_ENV , GL_TEXTURE_ENV_MODE , GL_MODULATE);
  glBindTexture(GL_TEXTURE_2D, textures[0]);
  glBegin(GL_QUADS);
    // left wall
    glNormal3d(1, 0, 0);
    glTexCoord2f(0.0,0.0); glVertex3f(0.1, 0.0, 0.1);
    glTexCoord2f(0.3,0.0); glVertex3f(0.1, 0.0, 0.9);
    glTexCoord2f(0.3,1.0); glVertex3f(0.1, 0.8, 0.9);
    glTexCoord2f(0.0,1.0); glVertex3f(0.1, 0.8, 0.1);
    // front wall
    glNormal3d(0, 0, -1);
    glTexCoord2f(0.0, 0.0); glVertex3f(0.1, 0.0, 0.9);
    glTexCoord2f(1.0, 0.0); glVertex3f(0.9, 0.0, 0.9);
    glTexCoord2f(1.0, 1.0); glVertex3f(0.9, 0.8, 0.9);
    glTexCoord2f(0.0, 1.0); glVertex3f(0.1, 0.8, 0.9);
    // right wall
    glNormal3d(-1, 0, 0);
    glTexCoord2f(0.0, 0.0); glVertex3f(0.9, 0.0, 0.9);
    glTexCoord2f(1.0, 0.0); glVertex3f(0.9, 0.0, 0.1);
    glTexCoord2f(1.0, 1.0); glVertex3f(0.9, 0.8, 0.1);
    glTexCoord2f(0.0, 1.0); glVertex3f(0.9, 0.8, 0.9);
    // back wall
    glNormal3d(0, 0, -1);
    glTexCoord2f(0.0, 0.0); glVertex3f(0.0, 0.0, 0.0);
    glTexCoord2f(1.0, 0.0); glVertex3f(0.0, 0.0, 0.0);
    glTexCoord2f(1.0, 1.0); glVertex3f(0.0, 0.0, 0.0);
    glTexCoord2f(0.0, 1.0); glVertex3f(0.0, 0.0, 0.0);
    // end
  glEnd();

  glBindTexture(GL_TEXTURE_2D, textures[1]);
  glColor3ub(0x77, 0x77 , 0x77); //TODO: could this be a triangle strip?
  glBegin(GL_TRIANGLES); //TODO: redo roof normal vectors with new points
    // left roof
    // glNormal3f(0.37, 0.93, 0);
    glTexCoord2f(0.0, 0.0); glVertex3d(0.0, 0.8, 0.0);
    glTexCoord2f(1.0, 0.0); glVertex3d(0.0, 0.8, 1.0);
    glTexCoord2f(0.5, 1.0); glVertex3d(0.5, 1.0, 0.5);
    // front roof
    // glNormal3f(0, 0.93, 0.37);
    glTexCoord2f(0.0, 0.0); glVertex3d(0.0, 0.8, 1.0);
    glTexCoord2f(1.0, 0.0); glVertex3d(1.0, 0.8, 1.0);
    glTexCoord2f(0.5, 1.0); glVertex3d(0.5, 1.0, 0.5);
    // right roof
    // glNormal3f(-0.37, 0.93, 0);
    glTexCoord2f(0.0, 0.0); glVertex3d(1.0, 0.8, 1.0);
    glTexCoord2f(1.0, 0.0); glVertex3d(1.0, 0.8, 0.0);
    glTexCoord2f(0.5, 1.0); glVertex3d(0.5, 1.0, 0.5);
    // back roof
    // glNormal3f(0, 0.93, -0.37);
    glTexCoord2f(0.0, 0.0); glVertex3d(1.0, 0.8, 0.0);
    glTexCoord2f(1.0, 0.0); glVertex3d(0.0, 0.8, 0.0);
    glTexCoord2f(0.5, 1.0); glVertex3d(0.5, 1.0, 0.5);

  glEnd();
  glDisable(GL_TEXTURE_2D);

  glPopMatrix();
}
