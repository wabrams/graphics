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

#include "framework.h"
#include "portal.h"
#include "shapes.h"

#define SHOW_AXES 1

#define NUMBER_PORTALS 2
#define NUMBER_CUBOIDS 2
#define BUFF_WRITE_LEN 8192
#define PROJECT_FONT GLUT_BITMAP_8_BY_13 //alt: GLUT_BITMAP_HELVETICA_12

window_t w;
view_t v;
player_t p;
portal_t portals[NUMBER_PORTALS];
cuboid_t cuboids[NUMBER_CUBOIDS];

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
  glTranslatef(-p.x, -p.y, -p.z);

  glEnable(GL_CULL_FACE);
  glShadeModel(GL_SMOOTH);

  //draw cubes
  for (int i = 0; i < 1; i++)
    drawCuboid(&cuboids[i]);

  //draw portals
  glBegin(GL_QUADS);
  for (int i = 0; i < NUMBER_PORTALS; i++)
  {
    glColor3ub(portals[i].color.r, portals[i].color.g, portals[i].color.b);
    for (int j = 0; j < 4; j++)
      glVertex3d(portals[i].vertices[j].x, portals[i].vertices[j].y, portals[i].vertices[j].z);
  }
  glEnd();

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

  info(5, 5, "this is a test");

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
      p.x += p.speed * Sin(p.yaw);
      p.z -= p.speed * Cos(p.yaw);
      break;
    case 'A':
    case 'a':
      p.x -= p.speed * Sin(p.yaw + 90);
      p.z += p.speed * Cos(p.yaw + 90);
      break;
    case 's':
    case 'S':
      p.x -= p.speed * Sin(p.yaw);
      p.z += p.speed * Cos(p.yaw);
      break;
    case 'D':
    case 'd':
      p.x -= p.speed * Sin(p.yaw - 90);
      p.z += p.speed * Cos(p.yaw - 90);
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

}

int main(int argc,char* argv[])
{
  w.width = w.height = 500;

  p.x = p.z = 1;
  p.y = 0.5;
  p.pitch = p.yaw = 0;
  p.speed = 0.05;

  v.th = v.ph = 0; //unused for now, not until 3P POV
  v.dim = 1;
  v.fov = 50;

  portals[0].color       = (color_t) {0x00, 0x00, 0xFF};
  portals[0].vertices[0] = (vertex_t){0, 0, 0};
  portals[0].vertices[1] = (vertex_t){1, 0, 0};
  portals[0].vertices[2] = (vertex_t){1, 1, 0};
  portals[0].vertices[3] = (vertex_t){0, 1, 0};

  portals[1].color       = (color_t) {0xFF, 0xAB, 0x00};
  portals[1].vertices[0] = (vertex_t){2, 0, 0};
  portals[1].vertices[1] = (vertex_t){3, 0, 0};
  portals[1].vertices[2] = (vertex_t){3, 1, 0};
  portals[1].vertices[3] = (vertex_t){2, 1, 0};

  cuboids[0].x = cuboids[0].y = cuboids[0].z = 0.5;
  cuboids[0].dimx = cuboids[0].dimy = cuboids[0].dimz = 0.1;
  cuboids[0].th = 0;
  cuboids[0].colors[0] = (color_t) {0xFF, 0x00, 0x00};
  cuboids[0].colors[1] = (color_t) {0x00, 0xFF, 0xFF};
  cuboids[0].colors[2] = (color_t) {0xFF, 0xFF, 0x00};
  cuboids[0].colors[3] = (color_t) {0x00, 0xFF, 0x00};
  cuboids[0].colors[4] = (color_t) {0x00, 0x00, 0xFF};
  cuboids[0].colors[5] = (color_t) {0xFF, 0x00, 0xFF};

  glutInit(&argc,argv);
  glutInitWindowSize(500,500);
  glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
  glutCreateWindow("HW #3 - William Abrams");
  glutIdleFunc(idle);
  glutDisplayFunc(display);
  glutReshapeFunc(reshape);
  glutKeyboardFunc(keyboard);
  glutSpecialFunc(special);
  glutMainLoop();
  return 0;
}
