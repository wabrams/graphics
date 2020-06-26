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

#define Cos(x) (cos((x)*3.1415927/180))
#define Sin(x) (sin((x)*3.1415927/180))
#define Tan(x) (tan((x)*3.1415927/180))
#define UNUSED(x) (void)(x)

#define BUFF_WRITE_LEN 8192
#define PROJECT_FONT GLUT_BITMAP_8_BY_13 //alt: GLUT_BITMAP_HELVETICA_12

window_t w;
view_t v;
player_t p;

void check(const char * where)
{
  int err = glGetError();
  if (err) fprintf(stderr, "ERROR: %s [%s]\n", gluErrorString(err), where);
}

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
}

void projection()
{
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  gluPerspective(v.fov, w.asp, v.dim / 4, 4 * v.dim);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}
void reshape(int width, int height)
{
  w.width = width;
  w.height = height;
  w.asp = 1.0 * width / height;

  glViewport(0, 0, width, height);
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

  glShadeModel(GL_SMOOTH);

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
    case 'W':
    case 'w':
      p.x += p.speed *  Sin(p.yaw);
      p.z += p.speed * -Cos(p.yaw);
      break;
    case 'A':
    case 'a':
      break;
    case 's':
    case 'S':
      p.x += p.speed * -Sin(p.yaw);
      p.z += p.speed *  Cos(p.yaw);
      break;
    case 'D':
    case 'd':

      break;
  }

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

  glutPostRedisplay();
}
void idle()
{

}

int main(int argc,char* argv[])
{
  w.width = w.height = 500;
  p.x = p.z = 1;
  p.y = 0;
  p.pitch = p.yaw = 0;
  p.speed = 0.05;

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
