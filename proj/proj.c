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

#define SHOW_AXES 0
#define SHOW_RD   10 //render distance

#define NUMBER_CUBOIDS 2
#define BUFF_WRITE_LEN 8192
#define PROJECT_FONT GLUT_BITMAP_8_BY_13 //alt: GLUT_BITMAP_HELVETICA_12

window_t w;
view_t v;
car_t c;
grid_t g;
light_t l;
int dr = 0;
float sun_dist   =  1;
float sun_angle  =  0;
float sun_height =  1.5;
bool daytime = true;
color_t skyday, skynight;

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

  glRotatef(-c.ph, 1, 0, 0);
  glRotatef(-c.th - dr,   0, 1, 0);
  glTranslatef(-c.x - (c.dist * Sin(c.th + dr)), -c.y - c.height, -c.z - (c.dist * Cos(c.th + dr)));
  glShadeModel(GL_SMOOTH);
  glEnable(GL_CULL_FACE);
  // color and position vectors
  float Ambient[]   = {(float)0.01*l.ambient,
                       (float)0.01*l.ambient,
                       (float)0.01*l.ambient,
                       1.0};
  float Diffuse[]   = {(float)0.01*l.diffuse,
                       (float)0.01*l.diffuse,
                       (float)0.01*l.diffuse,
                       1.0};
  float Specular[]  = {(float)0.01*l.specular,
                       (float)0.01*l.specular,
                       (float)0.01*l.specular,
                       1.0};
  float Position[]  = {sun_dist*(float)Cos(sun_angle),
                       sun_height,
                       sun_dist*(float)Sin(sun_angle),
                       1.0};
  vertex_t sun_pos = (vertex_t) {Position[0], Position[1], Position[2]};
  //  OpenGL should normalize normal vectors
  glEnable(GL_NORMALIZE);

  //  Draw light position as ball (still no lighting here)
  glColor3f(1,1,1);
  if (daytime)
    drawSun(&sun_pos, 0.2, l.emission, l.shiny, 4);

  //  Enable lighting
  glEnable(GL_LIGHTING);
  glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, true);
  glColorMaterial(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE);
  glEnable(GL_COLOR_MATERIAL);
  if (daytime)
    glEnable(GL_LIGHT0);
  else
    glDisable(GL_LIGHT0);
  glLightfv(GL_LIGHT0,GL_AMBIENT ,Ambient);
  glLightfv(GL_LIGHT0,GL_DIFFUSE ,Diffuse);
  glLightfv(GL_LIGHT0,GL_SPECULAR,Specular);
  glLightfv(GL_LIGHT0,GL_POSITION,Position);
  //TODO: glLightfv for headlights

  //draw car
  glDisable(GL_CULL_FACE);
  drawCar(&c);
  glEnable(GL_CULL_FACE);

  //draw grids
  for (int i = -SHOW_RD; i < SHOW_RD; i++)
    for (int j = -SHOW_RD; j < SHOW_RD; j++)
    {
      grid_t t = (grid_t){g.x + i, g.y, g.z + j};
      drawGrid(&t);
    }

  glDisable(GL_LIGHTING);
  vertex_t v = (vertex_t){c.x, c.y, c.z};
  drawSkybox(&v, 4.0, daytime ? &skyday : &skynight);
  glColor3f(1, 1, 1);
  #if SHOW_AXES
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

  info(5, 5, "GRID: (%3d, %3d, %3d), CPOS: (%.2f, %.2f), %.2f, VEL: %.2f", g.x, g.y, g.z, c.x, c.y, c.z, c.vel);
  info(5, 20, "DIST: %f, HEIGHT: %f, PH: %d", c.dist, c.height, c.ph);

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
    case 32:
      c.vel = 0;
      break;
    case '0':
      c.vel = 0;
      c.x = c.z = 0;
      dr = 0;
      break;
    case 'W':
    case 'w':
      if (c.vel <  0.99)
        c.vel += c.acc;
      break;
    case 'A':
    case 'a':
      c.th = fmod(c.th + 5.0, 360.0);
      // c.vel += (c.vel == 0)? 0: (c.vel > 0.01)? -0.01 : 0.01;
      break;
    case 's':
    case 'S':
      if (c.vel > -0.99)
        c.vel -= c.acc;
      break;
    case 'D':
    case 'd':
      c.th = fmod(c.th - 5.0, 360.0);
      // c.vel += (c.vel == 0)? 0: (c.vel > 0.01)? -0.01 : 0.01;
      break;
    case '+':
    case '=':
      if (c.dist > 0)
        c.dist -= 0.1;
      break;
    case '-':
    case '_':
      if (c.dist < 2)
        c.dist += 0.1;
      break;
    case '{':
    case '[':
      if (c.height > 0.1)
        c.height -= 0.1;
      break;
    case ']':
    case '}':
      if (c.height < 1.0)
        c.height += 0.1;
      break;
    case 'n':
    case 'N':
      daytime = !daytime;
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
      c.ph += 5; //TODO: bound
      break;
    case GLUT_KEY_LEFT:
      dr += 5;
      break;
    case GLUT_KEY_DOWN:
      c.ph -= 5;
      break;
    case GLUT_KEY_RIGHT:
      dr -= 5;
      break;
    case GLUT_KEY_PAGE_UP:
      v.fov -= 1;
      break;
    case GLUT_KEY_PAGE_DOWN:
      v.fov += 1;
      break;
  }

  check("func-special");
  projection();
  glutPostRedisplay();
}
void idle()
{
  double t = glutGet(GLUT_ELAPSED_TIME)/1000.0;
  c.z -= c.vel * t * Cos(c.th) / 100.0;
  c.x -= c.vel * t * Sin(c.th) / 100.0;
  vertex_t cloc = (vertex_t){c.x, c.y, c.z};
  getCurrentGrid(&cloc, &g);
  sun_angle = fmod(90*t,360.0);
  glutPostRedisplay();
}

int main(int argc,char* argv[])
{
  w.width = w.height = 600;

  c.x = c.y = c.z = 0.0;
  c.dimx = 0.05;
  c.dimy = 0.05;
  c.dimz = 0.1;
  c.th  = 0;
  c.ph = -25;
  c.vel = 0;
  c.acc = 0.005;
  c.dist = 1.4;
  c.height = 0.8;

  v.th = v.ph = 0; //unused for now, not until 3P POV
  v.dim = 4;
  v.fov = 40;

  l.emission = 0;
  l.ambient = 10;
  l.diffuse = 50;
  l.specular = 0;
  l.shininess = 0;
  l.shiny = 1;
  // #2b64c4
  skyday.r = 0x2b;
  skyday.g = 0x64;
  skyday.b = 0xc4;
  // #2e2083
  skynight.r = 0x2e;
  skynight.g = 0x20;
  skynight.b = 0x83;

  glutInit(&argc,argv);
  glutInitWindowSize(w.width,w.height);
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
