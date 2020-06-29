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

#define NUM_TEXTURES 5
const char * texture_filepaths[] = {"wall.bmp", "roof.bmp", "grass.bmp", "sky0.bmp", "sky1.bmp"};
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

void drawSkybox(vertex_t * v, float D)
{
  glPushMatrix();
  glTranslated(v -> x, v -> y, v -> z);
  glEnable(GL_TEXTURE_2D);
  glColor3f(1, 1, 1);
  //  Sides
  glBindTexture(GL_TEXTURE_2D,textures[3]);
  glBegin(GL_QUADS);
    glTexCoord2f(0.00,0); glVertex3f(-D,-D,-D);
    glTexCoord2f(0.25,0); glVertex3f(+D,-D,-D);
    glTexCoord2f(0.25,1); glVertex3f(+D,+D,-D);
    glTexCoord2f(0.00,1); glVertex3f(-D,+D,-D);

    glTexCoord2f(0.25,0); glVertex3f(+D,-D,-D);
    glTexCoord2f(0.50,0); glVertex3f(+D,-D,+D);
    glTexCoord2f(0.50,1); glVertex3f(+D,+D,+D);
    glTexCoord2f(0.25,1); glVertex3f(+D,+D,-D);

    glTexCoord2f(0.50,0); glVertex3f(+D,-D,+D);
    glTexCoord2f(0.75,0); glVertex3f(-D,-D,+D);
    glTexCoord2f(0.75,1); glVertex3f(-D,+D,+D);
    glTexCoord2f(0.50,1); glVertex3f(+D,+D,+D);

    glTexCoord2f(0.75,0); glVertex3f(-D,-D,+D);
    glTexCoord2f(1.00,0); glVertex3f(-D,-D,-D);
    glTexCoord2f(1.00,1); glVertex3f(-D,+D,-D);
    glTexCoord2f(0.75,1); glVertex3f(-D,+D,+D);
  glEnd();
  //  Top and bottom
  glBindTexture(GL_TEXTURE_2D,textures[4]);
  glBegin(GL_QUADS);
  glTexCoord2f(0.0,0); glVertex3f(+D,+D,-D);
  glTexCoord2f(0.5,0); glVertex3f(+D,+D,+D);
  glTexCoord2f(0.5,1); glVertex3f(-D,+D,+D);
  glTexCoord2f(0.0,1); glVertex3f(-D,+D,-D);

  glTexCoord2f(1.0,1); glVertex3f(-D,-D,+D);
  glTexCoord2f(0.5,1); glVertex3f(+D,-D,+D);
  glTexCoord2f(0.5,0); glVertex3f(+D,-D,-D);
  glTexCoord2f(1.0,0); glVertex3f(-D,-D,-D);

  glEnd();
  glDisable(GL_TEXTURE_2D);
  glPopMatrix();
}

static void drawHouse()
{
  glEnable(GL_TEXTURE_2D);
  glTexEnvi(GL_TEXTURE_ENV , GL_TEXTURE_ENV_MODE , GL_MODULATE);
  glBindTexture(GL_TEXTURE_2D, textures[0]);
  glBegin(GL_QUADS);
    // left wall
    glNormal3d(1, 0, 0);
    glTexCoord2f(0.0,0.0); glVertex3f(0.1, 0.0, 0.1);
    glTexCoord2f(1.0,0.0); glVertex3f(0.1, 0.0, 0.9);
    glTexCoord2f(1.0,1.0); glVertex3f(0.1, 0.8, 0.9);
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
    glNormal3f(-0.2,  0.5,  0.0);
    glTexCoord2f(0.0, 0.0); glVertex3d(0.0, 0.8, 0.0);
    glTexCoord2f(1.0, 0.0); glVertex3d(0.0, 0.8, 1.0);
    glTexCoord2f(0.5, 1.0); glVertex3d(0.5, 1.0, 0.5);
    // front roof
    glNormal3f( 0.0, -0.5, -0.2);
    glTexCoord2f(0.0, 0.0); glVertex3d(0.0, 0.8, 1.0);
    glTexCoord2f(1.0, 0.0); glVertex3d(1.0, 0.8, 1.0);
    glTexCoord2f(0.5, 1.0); glVertex3d(0.5, 1.0, 0.5);
    // right roof
    glNormal3f( 0.2,  0.5,  0.0);
    glTexCoord2f(0.0, 0.0); glVertex3d(1.0, 0.8, 1.0);
    glTexCoord2f(1.0, 0.0); glVertex3d(1.0, 0.8, 0.0);
    glTexCoord2f(0.5, 1.0); glVertex3d(0.5, 1.0, 0.5);
    // back roof
    glNormal3f( 0.0, -0.5,  0.2);
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
  glEnd();
  // green (grass)
  glEnable(GL_TEXTURE_2D);
  glTexEnvi(GL_TEXTURE_ENV , GL_TEXTURE_ENV_MODE , GL_MODULATE);
  glBindTexture(GL_TEXTURE_2D, textures[2]);
  glBegin(GL_QUADS);
  glColor3ub(0x00, 0x99, 0x00);
    glTexCoord2f(0.0, 0.0); glVertex3f(0.1, 0.0, 0.1);
    glTexCoord2f(4.0, 0.0); glVertex3f(0.1, 0.0, 0.9);
    glTexCoord2f(4.0, 4.0); glVertex3f(0.9, 0.0, 0.9);
    glTexCoord2f(0.0, 4.0); glVertex3f(0.9, 0.0, 0.1); //excess texcoord2f forces tiling
  glEnd();
  glDisable(GL_TEXTURE_2D);

  //generate the house per this grid
  color_t hcolor;
  // NW house
  glTranslated(0.2, 0.0, 0.2);
    hcolor.r = rand();
    hcolor.g = rand();
    hcolor.b = rand();
    glScalef(0.2, 0.2, 0.2);
    glColor3ub(hcolor.r, hcolor.g, hcolor.b);
    drawHouse();
    glScalef(5.0, 5.0, 5.0);
    glTranslated(-0.2, 0.0, -0.2);
  // SW house
  glTranslated(0.2, 0.0, 0.6);
    hcolor.r = rand();
    hcolor.g = rand();
    hcolor.b = rand();
    glScalef(0.2, 0.2, 0.2);
    glColor3ub(hcolor.r, hcolor.g, hcolor.b);
    drawHouse();
    glScalef(5.0, 5.0, 5.0);
    glTranslated(-0.2, 0.0, -0.6);
  // SE house
  glTranslated(0.6, 0.0, 0.6);
    hcolor.r = rand();
    hcolor.g = rand();
    hcolor.b = rand();
    glScalef(0.2, 0.2, 0.2);
    glColor3ub(hcolor.r, hcolor.g, hcolor.b);
    drawHouse();
    glScalef(5.0, 5.0, 5.0);
    glTranslated(-0.6, 0.0, -0.6);
  // NE house
  glTranslated(0.6, 0.0, 0.2);
    hcolor.r = rand();
    hcolor.g = rand();
    hcolor.b = rand();
    glScalef(0.2, 0.2, 0.2);
    glColor3ub(hcolor.r, hcolor.g, hcolor.b);
    drawHouse();
    glScalef(5.0, 5.0, 5.0);
    glTranslated(-0.6, 0.0, -0.2);
  glPopMatrix();
}
