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

void check(const char * where)
{
  int err = glGetError();
  if (err) fprintf(stderr, "ERROR: %s [%s]\n", gluErrorString(err), where);
}

void fatal(const char * format , ...)
{
   va_list args;
   va_start(args,format);
   vfprintf(stderr,format,args);
   va_end(args);
   exit(1);
}

void reverseBytes(void * x, const int n)
{
   char * ch = (char *) x;
   for (int k = 0; k < n/2; k++)
   {
      char tmp = ch[k];
      ch[k] = ch[n-1-k];
      ch[n-1-k] = tmp;
   }
}

unsigned int loadTextureFromBMP(const char * file)
{
  unsigned int   texture;    // Texture name
  FILE*          f;          // File pointer
  unsigned short magic;      // Image magic
  unsigned int   dx,dy,size; // Image dimensions
  unsigned short nbp,bpp;    // Planes and bits per pixel
  unsigned char* image;      // Image data
  unsigned int   off;        // Image offset
  unsigned int   k;          // Counter
  int            max;        // Maximum texture dimensions

  //  Open file
  f = fopen(file,"rb");
  if (!f) fatal("Cannot open file %s\n",file);
  //  Check image magic
  if (fread(&magic,2,1,f)!=1) fatal("Cannot read magic from %s\n",file);
  if (magic!=0x4D42 && magic!=0x424D) fatal("Image magic not BMP in %s\n",file);
  //  Read header
  if (fseek(f,8,SEEK_CUR) || fread(&off,4,1,f)!=1 ||
      fseek(f,4,SEEK_CUR) || fread(&dx,4,1,f)!=1 || fread(&dy,4,1,f)!=1 ||
      fread(&nbp,2,1,f)!=1 || fread(&bpp,2,1,f)!=1 || fread(&k,4,1,f)!=1)
    fatal("Cannot read header from %s\n",file);
  //  Reverse bytes on big endian hardware (detected by backwards magic)
  if (magic==0x424D)
  {
    reverseBytes(&off,4);
    reverseBytes(&dx,4);
    reverseBytes(&dy,4);
    reverseBytes(&nbp,2);
    reverseBytes(&bpp,2);
    reverseBytes(&k,4);
  }
  //  Check image parameters
  glGetIntegerv(GL_MAX_TEXTURE_SIZE,&max);
  if (dx<1 || dx>(unsigned int)max) fatal("%s image width %d out of range 1-%d\n",file,dx,max);
  if (dy<1 || dy>(unsigned int)max) fatal("%s image height %d out of range 1-%d\n",file,dy,max);
  if (nbp!=1)  fatal("%s bit planes is not 1: %d\n",file,nbp);
  if (bpp!=24) fatal("%s bits per pixel is not 24: %d\n",file,bpp);
  if (k!=0)    fatal("%s compressed files not supported\n",file);

  #ifndef GL_VERSION_2_0
    //  OpenGL 2.0 lifts the restriction that texture size must be a power of two
    for (k=1;k<dx;k*=2);
    if (k!=dx) fatal("%s image width not a power of two: %d\n",file,dx);
    for (k=1;k<dy;k*=2);
    if (k!=dy) fatal("%s image height not a power of two: %d\n",file,dy);
  #endif

  //  Allocate image memory
  size = 3*dx*dy;
  image = (unsigned char*) malloc(size);
  if (!image) fatal("Cannot allocate %d bytes of memory for image %s\n",size,file);
  //  Seek to and read image
  if (fseek(f,off,SEEK_SET) || fread(image,size,1,f)!=1) fatal("Error reading data from image %s\n",file);
  fclose(f);
  //  Reverse colors (BGR -> RGB)
  for (k=0;k<size;k+=3)
  {
    unsigned char temp = image[k];
    image[k]   = image[k+2];
    image[k+2] = temp;
  }

  //  Sanity check
  check("func-loadTextureFromBMP");
  //  Generate 2D texture
  glGenTextures(1,&texture);
  glBindTexture(GL_TEXTURE_2D,texture);
  //  Copy image
  glTexImage2D(GL_TEXTURE_2D,0,3,dx,dy,0,GL_RGB,GL_UNSIGNED_BYTE,image);
  if (glGetError()) fatal("Error in glTexImage2D %s %dx%d\n",file,dx,dy);
  //  Scale linearly when image size doesn't match
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);

  //  Free image memory
  free(image);
  //  Return texture name
  return texture;
}

void getCurrentGrid(vertex_t * v, grid_t * g)
{
  g -> x = (int) ((v -> x < 0)? (v -> x - 1): (v -> x));
  g -> y = (int) ((v -> y < 0)? (v -> y - 1): (v -> y));
  g -> z = (int) ((v -> z < 0)? (v -> z - 1): (v -> z));
}
