#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdbool.h>
#include "lorenz.h"
#include "color.h"
#define GL_GLEXT_PROTOTYPES
#ifdef __APPLE__
  #include <GLUT/glut.h>
#else
  #include <GL/glut.h>
#endif

//to mute the unused paremeter warnings for mouse position in key and special functions
//https://stackoverflow.com/questions/3599160/how-to-suppress-unused-parameter-warnings-in-c
#define UNUSED(x) (void)(x)

#define BUF_L  4096
#define DISPLAY_PRESET_THETA -50
#define DISPLAY_PRESET_PHI   -25
#define DISPLAY_PRESET_W      25

////////////////////////////////////////////////////////////////
//display
bool a = true;
color_t color;
int th = DISPLAY_PRESET_THETA;  // azimuth of view angle (theta)
int ph = DISPLAY_PRESET_PHI;    // elevation of view angle (phi)
double w = DISPLAY_PRESET_W;    // 4D, used for zoom
double dim = 2;                 // dimension of orthogonal box
//lorenz
lorenz_t lorenz;
////////////////////////////////////////////////////////////////

// reused this from print function (just renamed it to what i prefer), i tried and tried to think of better ways to do this but this is just awesome
void label(const char * format, ...)
{
  char buf[BUF_L];
  //parse variadic
  va_list args;
  va_start(args, format);
  vsnprintf(buf, BUF_L, format, args);
  va_end(args);
  //display characters
  char * ch = buf;
  while (*ch)
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *ch++);
}
// reused errcheck function from ex5 in class
void ErrCheck(char * where)
{
   int err = glGetError();
   if (err) fprintf(stderr,"ERROR: %s [%s]\n",gluErrorString(err),where);
}
// reused / heavily modified ex6 from class
void display()
{
  //  Clear the image
  glClear(GL_COLOR_BUFFER_BIT);
  //  Reset previous transforms
  glLoadIdentity();
  //  Set view angle
  glRotated(ph,1,0,0);
  glRotated(th,0,1,0);
  //  Draw 10 pixel yellow points
  glColor3f(color.r,color.g,color.b);
  glBegin(GL_LINE_STRIP);
  //the upcoming segment is upcycled from lorenz.c provided
  //i have double checked this with the wikipedia page on lorenz
  double x = 0.5;
  double y = 0.5;
  double z = 0.5;
  double dt = 0.001;
  for (int i = 0; i < 50000; i++)
  {
    double dx = lorenz.s*(y-x);
    double dy = x*(lorenz.r-z)-y;
    double dz = x*y - lorenz.b*z;
    x += dt*dx;
    y += dt*dy;
    z += dt*dz;
    glVertex4d(x, y, z, w);
  }
  glEnd();
  glColor3f(1,1,1);
  // draw the axes, if enabled
  if (a)
  {
    //i felt like having axes in positive and negative directions
    glBegin(GL_LINES);
    glVertex3d( 0, 0, 0);
    glVertex3d(-1, 0, 0);
    glVertex3d( 0, 0, 0);
    glVertex3d( 1, 0, 0);
    glVertex3d( 0, 0, 0);
    glVertex3d( 0,-1, 0);
    glVertex3d( 0, 0, 0);
    glVertex3d( 0, 1, 0);
    glVertex3d( 0, 0, 0);
    glVertex3d( 0, 0,-1);
    glVertex3d( 0, 0, 0);
    glVertex3d( 0, 0, 1);
    glEnd();
    //  Label axes
    glRasterPos3d(-1, 0, 0);
    label("-X");
    glRasterPos3d( 1, 0, 0);
    label("+X");
    glRasterPos3d( 0,-1, 0);
    label("-Y");
    glRasterPos3d( 0, 1, 0);
    label("+Y");
    glRasterPos3d( 0, 0,-1);
    label("-Z");
    glRasterPos3d( 0, 0, 1);
    label("+Z");
  }

  //  display parameters
  glWindowPos2i(10,10); //from bottom left corner
  label("Angle = (%3d, %3d)  w=%f, r=%2.2f, s=%2.2f, b=%2.2f", th, ph, w, lorenz.r, lorenz.s, lorenz.b);
  // check for errors
  ErrCheck("display");
  //  Flush and swap
  glFlush();
  glutSwapBuffers();
}

void key(unsigned char ch,int x,int y)
{
  UNUSED(x);
  UNUSED(y);

  switch (ch)
  {
    case 27: //escape
      exit(0);
      break;
    case '0':
      th = ph = 0;
      break;
    case '1':
      lorenz.r = LORENZ_P1_RHO;
      lorenz.s = LORENZ_P1_SIGMA;
      lorenz.b = LORENZ_P1_BETA;
      th = DISPLAY_PRESET_THETA;
      ph = DISPLAY_PRESET_PHI;
      w = DISPLAY_PRESET_W;
      break;
    case '2':
      lorenz.r = LORENZ_P2_RHO;
      lorenz.s = LORENZ_P2_SIGMA;UNUSED(x); //surpress unused parameter warnings
      lorenz.b = LORENZ_P2_BETA;
      th = DISPLAY_PRESET_THETA;
      ph = DISPLAY_PRESET_PHI;
      w = DISPLAY_PRESET_W;
      break;
    case '4':
      if (color.r > 0) color.r -= 0.1;
      break;
    case '5':
      if (color.g > 0) color.g -= 0.1;
      break;
    case '6':
      if (color.b > 0) color.b -= 0.1;
      break;
    case '7':
      if (color.r < 1) color.r += 0.1;
      break;
    case '8':
      if (color.g < 1) color.g += 0.1;
      break;
    case '9':
      if (color.b < 1) color.b += 0.1;
      break;
    case '+':
    case '=':
      w += 0.5;
      break;
    case '-':
    case '_':
      w -= 0.5;
      break;
    case 'a':
    case 'A':
      a = !a;
      break;
    case 'r':
      lorenz.r -= 0.01;
      break;
    case 'R':
      lorenz.r += 0.01;
      break;
    case 's':
      lorenz.s -= 0.01;
      break;
    case 'S':
      lorenz.s += 0.01;
      break;
    case 'b':
      lorenz.b -= 0.01;
      break;
    case 'B':
      lorenz.b += 0.01;
      break;
    case 'c':
    case 'C':
      color.r = COLOR_R_DEFAULT;
      color.g = COLOR_G_DEFAULT;
      color.b = COLOR_B_DEFAULT;
      break;
  }
  glutPostRedisplay();
}

void special(int key, int x, int y)
{
  UNUSED(x);
  UNUSED(y);

  switch (key)
  {
    case GLUT_KEY_UP:
      ph += 5;
      break;
    case GLUT_KEY_DOWN:
      ph -= 5;
      break;
    case GLUT_KEY_LEFT:
      th -= 5;
      break;
    case GLUT_KEY_RIGHT:
      th += 5;
      break;
  }

  th %= 360;
  ph %= 360;

  glutPostRedisplay();
}
//reused from ex6 in class
void reshape(int width, int height)
{
   double w2h = (height>0) ? (double)width/height : 1;
   glViewport(0,0, width,height);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   //glOrtho(left, right, bottom, top, nearVal, farVal)
   glOrtho(-dim*w2h,+dim*w2h, -dim, +dim, -dim, +dim);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
}

int main(int argc, char * argv[])
{
  //initialize globals
  lorenz.r = LORENZ_P1_RHO;
  lorenz.s = LORENZ_P1_SIGMA;
  lorenz.b = LORENZ_P1_BETA;
  color.r = COLOR_R_DEFAULT;
  color.g = COLOR_G_DEFAULT;
  color.b = COLOR_B_DEFAULT;

  glutInit(&argc,argv);
  // Request double buffered, true color window
  glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
  glutInitWindowSize(500,500);
  glutCreateWindow("HW1 - William Abrams");
  glutDisplayFunc(display);
  glutReshapeFunc(reshape);
  glutSpecialFunc(special);
  glutKeyboardFunc(key);
  // Pass control to GLUT so it can interact with the user
  glutMainLoop();

  return 0;
}
