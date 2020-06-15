#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#define GL_GLEXT_PROTOTYPES
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include "shapes.h"
#include "trig.h"
#include "structs.h"

#define UNUSED(x) (void)(x)

enum viewmode_e
{
  MODE_OVERHEAD_ORTHOGONAL,
  MODE_OVERHEAD_PERSPECTIVE,
  MODE_FIRSTPERSON_PERSPECTIVE,
};
typedef enum viewmode_e viewmode_t;

ccoord_t p; //1st person pov (player)
window_t win;
view_t v;
viewmode_t mode = MODE_OVERHEAD_ORTHOGONAL;
const char * mtext[] = {"OH_ORTH", "OH_PRSP", "1P_PRSP"};

// reused errcheck function from ex5 in class
void ErrCheck(char * where)
{
   int err = glGetError();
   if (err) fprintf(stderr,"ERROR: %s [%s]\n",gluErrorString(err),where);
}

/*
 *  Convenience routine to output raster text
 *  Use VARARGS to make this more flexible
 */
#define LEN 8192  //  Maximum length of text string
void Print(const char* format , ...)
{
   char    buf[LEN];
   char*   ch=buf;
   va_list args;
   //  Turn the parameters into a character string
   va_start(args,format);
   vsnprintf(buf,LEN,format,args);
   va_end(args);
   //  Display the characters one at a time at the current raster position
   while (*ch)
      glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,*ch++);
}

/*
 *  Set projection
 */
void projection()
{
  //  Tell OpenGL we want to manipulate the projection matrix
  glMatrixMode(GL_PROJECTION);
  //  Undo previous transformations
  glLoadIdentity();
  //  Perspective transformation
  if (mode == MODE_OVERHEAD_PERSPECTIVE || mode == MODE_FIRSTPERSON_PERSPECTIVE)
    gluPerspective(v.fov,v.asp,v.dim/4,4*v.dim);
  //  Orthogonal projection
  else if (mode == MODE_OVERHEAD_ORTHOGONAL)
    glOrtho(-v.asp*v.dim,+v.asp*v.dim, -v.dim,+v.dim, -v.dim,+v.dim);
  //  Switch to manipulating the model matrix
  glMatrixMode(GL_MODELVIEW);
  //  Undo previous transformations
  glLoadIdentity();
}

/*
 *  OpenGL (GLUT) calls this routine to display the scene
 */
void display()
{
  const double len=1.0;  //  Length of axes
  double Ex, Ey, Ez;
  //  Erase the window and the depth buffer
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  //  Enable Z-buffering in OpenGL
  glEnable(GL_DEPTH_TEST);
  //  Undo previous transformations
  glLoadIdentity();
  // perspective
  switch (mode)
  {
    case MODE_OVERHEAD_ORTHOGONAL:
      glRotatef(v.ph,1,0,0);
      glRotatef(v.th,0,1,0);
      break;
    case MODE_OVERHEAD_PERSPECTIVE:
      Ex = -2*v.dim*Sin(v.th)*Cos(v.ph);
      Ey = +2*v.dim*Sin(v.ph);
      Ez = +2*v.dim*Cos(v.th)*Cos(v.ph);
      gluLookAt(Ex,Ey,Ez,0,0,0,0,Cos(v.ph),0);
      break;
    case MODE_FIRSTPERSON_PERSPECTIVE:
      glRotatef(p.pitch, 1, 0, 0);
      glRotatef(p.yaw, 0, 1, 0);
      glTranslatef(-p.c.x, -p.c.y, -p.c.z);
      break;
    default:
      fprintf(stderr, "illegal mode %d!\n", mode);
      exit(0);
  }
  // enable face culling
  // glEnable(GL_CULL_FACE);
  // glCullFace(GL_BACK);
  // draw
  glColor3f(1,0,1);
  house( 0.2, 0.10, 0.3, 0.10, 0.10, 0.10,  0, 0.5, 0.3, 0.3);
  house(-0.2, 0.16, 0.1, 0.12, 0.16, 0.17, 15, 0.5, 0.5, 0.3);
  house(-0.6, 0.08,-0.4, 0.13, 0.08, 0.15, 30, 0.2, 0.5, 0.5);
  house( 0.7, 0.20,-0.3, 0.08, 0.20, 0.13, 45, 0.5, 0.2, 0.5);
  house(-0.5, 0.23, 0.6, 0.15, 0.23, 0.13, 73, 0.7, 0.7, 0.2);
  glColor3f(0, 1, 0);
  xzplane(1, 0, 1, 0, 0);
  //  White
  glColor3f(1,1,1);
  //  Draw axes
  if (v.axes)
  {
    glBegin(GL_LINES);
    glVertex3d(0.0,0.0,0.0);
    glVertex3d(len,0.0,0.0);
    glVertex3d(0.0,0.0,0.0);
    glVertex3d(0.0,len,0.0);
    glVertex3d(0.0,0.0,0.0);
    glVertex3d(0.0,0.0,len);
    glEnd();
    //  Label axes
    glRasterPos3d(len,0.0,0.0);
    Print("X");
    glRasterPos3d(0.0,len,0.0);
    Print("Y");
    glRasterPos3d(0.0,0.0,len);
    Print("Z");
  }
  //  Five pixels from the lower left corner of the window
  glWindowPos2i(5,5);
  //  Print the text string
  Print("VA=(%d,%d), DIM=%2.2f, FOV=%d, ASP=%2.2f [%s]", v.th, v.ph, v.dim, v.fov, v.asp, mtext[mode]);
  // Check for errors
  ErrCheck("display");
  //  Render the scene
  glFlush();
  //  Make the rendered scene visible
  glutSwapBuffers();
}

// GLUT calls this routine when the window is resized
void reshape(int width,int height)
{
  win.w = width;
  win.h = height;
  glViewport(0, 0, width,height);
  // Let projection handle the rest
  projection();
}

// GLUT calls this routine when an arrow key is pressed
void special(int key, int x, int y)
{
  UNUSED(x);
  UNUSED(y);

  switch (key)
  {
    case GLUT_KEY_UP:
      v.ph += 5;
      break;
    case GLUT_KEY_DOWN:
      v.ph -= 5;
      break;
    case GLUT_KEY_LEFT:
      v.th -= 5;
      break;
    case GLUT_KEY_RIGHT:
      v.th += 5;
      break;
    // default:
    //   printf("invalid key: %d\n", key);
    //   break;
  }

  //  Keep angles to +/-360 degrees
  v.th %= 360;
  v.ph %= 360;
  //  Tell GLUT it is necessary to redisplay the scene
  projection();
  glutPostRedisplay();
}

// GLUT calls this routine when a key is pressed
void key(unsigned char ch, int x, int y)
{
  UNUSED(x);
  UNUSED(y);

  switch (ch)
  {
    case 27:
    case 'q':
    case 'Q':
      // exit
      exit(0);
      break;
    case '0':
      // reset view
      v.th = v.ph = 0;
      v.dim = 2.5;
      v.fov = 50;
      break;
    case '1':
      mode = MODE_OVERHEAD_ORTHOGONAL;
      break;
    case '2':
      mode = MODE_OVERHEAD_PERSPECTIVE;
      break;
    case '3':
      mode = MODE_FIRSTPERSON_PERSPECTIVE;
      break;
    case 't':
    case 'T':
      // toggle axes
      v.axes = !v.axes;
      break;
    case 'r':
    case 'R':
      p.c.y += 0.05;
      break;
    case 'f':
    case 'F':
      p.c.y -= 0.05;
      break;
    case 'a':
    case 'A':
      p.yaw += 2;
      break;
    case 'd':
    case 'D':
      p.yaw -= 2;
      break;
    case 'm':
      // next display mode
      mode = (mode+1)%3;
      break;
    case 'M':
      // prev display mode
      mode = (mode+2)%3;
      break;
    case 'i':
    case 'I':
      v.fov += 1;
      break;
    case 'o':
    case 'O':
      v.fov -= 1;
      break;
    case '=':
    case '+':
      // zoom in
      if (v.dim > 0) v.dim -= 0.1;
      break;
    case '-':
    case '_':
      v.dim += 0.1;
      break;
    // default:
    //   printf("invalid chr: %d\n", ch);
    //   break;
  }
  reshape(win.w, win.h);
  glutPostRedisplay();
}

// GLUT calls this routine when there is nothing else to do
void idle()
{
   double t = glutGet(GLUT_ELAPSED_TIME)/1000.0;
   glutPostRedisplay();
}

/*
 *  Start up GLUT and tell it what to do
 */
int main(int argc,char* argv[])
{
  win.w = win.h = 500;

  v.th = -45;
  v.ph = +45;
  v.dim = 2.5;
  v.axes = false;
  v.fov = 55;
  v.asp = 1;

  p.pitch = 0;
  p.yaw = 0;
  p.c.x = p.c.z = 1;
  p.c.y = 0.1;
  //  Initialize GLUT and process user parameters
  glutInit(&argc,argv);
  //  Request double buffered, true color window with Z buffering at 600x600
  glutInitWindowSize(500,500);
  glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
  //  Create the window
  glutCreateWindow("HW #2 - William Abrams");
  //  Tell GLUT to call "idle" when there is nothing else to do
  glutIdleFunc(idle);
  //  Tell GLUT to call "display" when the scene should be drawn
  glutDisplayFunc(display);
  //  Tell GLUT to call "reshape" when the window is resized
  glutReshapeFunc(reshape);
  //  Tell GLUT to call "special" when an arrow key is pressed
  glutSpecialFunc(special);
  //  Tell GLUT to call "key" when a key is pressed
  glutKeyboardFunc(key);
  //  Pass control to GLUT so it can interact with the user
  glutMainLoop();
  return 0;
}
