#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdbool.h>
#include <math.h>

#define GL_GLEXT_PROTOTYPES
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include "framework.h"
#include "shapes.h"

#define SHOW_AXES 1
#define SHOW_RD   10 //render distance

#define NUMBER_CUBOIDS 2
#define BUFF_WRITE_LEN 8192
#define PROJECT_FONT GLUT_BITMAP_8_BY_13 //alt: GLUT_BITMAP_HELVETICA_12

window_t w;
view_t v;
player_t p;
cuboid_t car;
grid_t g;
light_t l;
float sun_dist   =  2;
float sun_angle  = 90;
float sun_height =  0.5;

void write(float x, float y, float z, const char * format, ...)
{
  glRasterPos3d(x, y, z);

  char buff[BUFF_WRITE_LEN];
  char * next = buff;

  va_list args;
  va_start(args, format);
  vsnprintf(buff, BUFF_WRITE_LEN, format, args);
  va_end(args);

  while (*next)
    glutBitmapCharacter(PROJECT_FONT, *next++);

  check("func-write");
}

void info(int x, int y, const char * format, ...)
{
  glWindowPos2i(x, y);

  char buff[BUFF_WRITE_LEN];
  char * next = buff;

  va_list args;
  va_start(args, format);
  vsnprintf(buff, BUFF_WRITE_LEN, format, args);
  va_end(args);

  while (*next)
    glutBitmapCharacter(PROJECT_FONT, *next++);

  check("func-info");
}

void projection()
{
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  gluPerspective(v.fov, w.asp, v.dim / 4, 4 * v.dim);

  check("func-projection");
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}
void reshape(int width, int height)
{
  w.width = width;
  w.height = height;
  w.asp = 1.0 * width / height;

  glViewport(0, 0, width, height);
  check("func-reshape");
  projection();
}
void display()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glEnable(GL_DEPTH_TEST);
  glLoadIdentity();

  glRotatef(p.pitch, 1, 0, 0);
  glRotatef(p.yaw,   0, 2, 0);
  glTranslatef(-car.x - p.x, -car.y - p.y, -car.z - p.z);

  glEnable(GL_CULL_FACE);
  glShadeModel(GL_SMOOTH);

  //translate intensity to color vectors
  float Ambient[]   = {0.01*l.ambient ,0.01*l.ambient ,0.01*l.ambient ,1.0};
  float Diffuse[]   = {0.01*l.diffuse ,0.01*l.diffuse ,0.01*l.diffuse ,1.0};
  float Specular[]  = {0.01*l.specular,0.01*l.specular,0.01*l.specular,1.0};
  //  Light position
  float Position[]  = {sun_dist*Cos(sun_angle),sun_height,sun_dist*Sin(sun_angle), 1};
  vertex_t sun_pos = (vertex_t) {Position[0], Position[1], Position[2]};
  //  Draw light position as ball (still no lighting here)
  glColor3f(1,1,1);
  drawSun(&sun_pos, 0.2, l.emission, l.shiny, 4);
  //  OpenGL should normalize normal vectors
  glEnable(GL_NORMALIZE);
  //  Enable lighting
  glEnable(GL_LIGHTING);
  glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, true);
  glColorMaterial(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE);
  glEnable(GL_COLOR_MATERIAL);
  glEnable(GL_LIGHT0);
  glLightfv(GL_LIGHT0,GL_AMBIENT ,Ambient);
  glLightfv(GL_LIGHT0,GL_DIFFUSE ,Diffuse);
  glLightfv(GL_LIGHT0,GL_SPECULAR,Specular);
  glLightfv(GL_LIGHT0,GL_POSITION,Position);


  //draw cube
  vertex_t v = (vertex_t){car.x, car.y, car.z};
  drawCuboid(&car);
  //draw grids
  for (int i = -SHOW_RD; i < SHOW_RD; i++)
    for (int j = -SHOW_RD; j < SHOW_RD; j++)
    {
      grid_t t = (grid_t){g.x + i, g.y, g.z + j};
      drawGrid(&t);
    }

  glDisable(GL_LIGHTING);
  drawSkybox(&v, 4.0);
  #if SHOW_AXES
    glColor3f(1, 1, 1);
    glBegin(GL_LINES);
    glVertex3d(0, 0, 0);
    glVertex3d(1, 0, 0);
    glVertex3d(0, 0, 0);
    glVertex3d(0, 1, 0);
    glVertex3d(0, 0, 0);
    glVertex3d(0, 0, 1);
    glEnd();

    write(1, 0, 0, "X");
    write(0, 1, 0, "Y");
    write(0, 0, 1, "Z");
  #endif

  info(5, 5, "GRID: (%3d, %3d, %3d)", g.x, g.y, g.z);

  check("func-display");
  glFlush();
  glutSwapBuffers();
}
void keyboard(unsigned char k, int x, int y)
{
  UNUSED(x);
  UNUSED(y);

  switch (k)
  {
    case 27:
    case 'Q':
    case 'q':
      exit(0);
      break;
    case 'W':
    case 'w':
      car.x += p.speed * Sin(p.yaw);
      car.z -= p.speed * Cos(p.yaw);
      break;
    case 'A':
    case 'a':
      car.x -= p.speed * Sin(p.yaw + 90);
      car.z += p.speed * Cos(p.yaw + 90);
      break;
    case 's':
    case 'S':
      car.x -= p.speed * Sin(p.yaw);
      car.z += p.speed * Cos(p.yaw);
      break;
    case 'D':
    case 'd':
      car.x -= p.speed * Sin(p.yaw - 90);
      car.z += p.speed * Cos(p.yaw - 90);
      break;
  }

  check("func-keyboard");
  glutPostRedisplay();
}
void special(int k, int x, int y)
{
  UNUSED(x);
  UNUSED(y);

  switch (k)
  {
    case GLUT_KEY_UP:
      p.pitch -= 4;
      if (p.pitch < 0)
        p.pitch += 360;
      break;
    case GLUT_KEY_LEFT:
      p.yaw -= 4;
      if (p.yaw < 0)
        p.yaw += 360;
      break;
    case GLUT_KEY_DOWN:
      p.pitch += 4;
      if (p.pitch >= 360)
        p.pitch -= 360;
      break;
    case GLUT_KEY_RIGHT:
      p.yaw += 4;
      if (p.yaw >= 360)
        p.yaw -= 360;
      break;
  }

  check("func-special");
  glutPostRedisplay();
}
void idle()
{
  double t = glutGet(GLUT_ELAPSED_TIME)/1000.0;
  car.th = fmod(90*t,360.0);
  vertex_t cloc = (vertex_t){car.x, car.y, car.z};
  getCurrentGrid(&cloc, &g);
  sun_angle = fmod(90*t,360.0);
  glutPostRedisplay();
}

int main(int argc,char* argv[])
{
  w.width = w.height = 500;

  p.x = 0;
  p.z = 1;
  p.y = 0.5;
  p.pitch = p.yaw = 0;
  p.speed = 0.05;

  v.th = v.ph = 0; //unused for now, not until 3P POV
  v.dim = 2;
  v.fov = 50;

  car.x = car.y = car.z = 0;
  car.dimx = car.dimy = car.dimz = 0.05;
  car.th = 0;
  car.colors[0] = (color_t) {0xFF, 0x00, 0x00};
  car.colors[1] = (color_t) {0x00, 0xFF, 0xFF};
  car.colors[2] = (color_t) {0xFF, 0xFF, 0x00};
  car.colors[3] = (color_t) {0x00, 0xFF, 0x00};
  car.colors[4] = (color_t) {0x00, 0x00, 0xFF};
  car.colors[5] = (color_t) {0xFF, 0x00, 0xFF};

  l.emission = 0;
  l.ambient = 10;
  l.diffuse = 50;
  l.specular = 0;
  l.shininess = 0;
  l.shiny = 1;

  glutInit(&argc,argv);
  glutInitWindowSize(500,500);
  glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
  glutCreateWindow("William Abrams - A Suburban Drive");
  glutIdleFunc(idle);
  glutDisplayFunc(display);
  glutReshapeFunc(reshape);
  glutKeyboardFunc(keyboard);
  glutSpecialFunc(special);

  loadTextures();

  glutMainLoop();
  return 0;
}
