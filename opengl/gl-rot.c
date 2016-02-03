/************
Bitmap manip borrowed from BeholderOf of vbforums.com, edited by ollo and Dragos Rizescu of StackOverflow
Axis rotations and base structure from user marshats of the opengl.org forums
*****************/
 
#include <GL/glut.h>
#include <GL/gl.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <stdint.h>

#define  RADDEG  57.29577951f
 
float XUP[3] = {1,0,0}, XUN[3] = {-1, 0, 0},
  YUP[3] = {0,1,0}, YUN[3] = { 0,-1, 0},
  ZUP[3] = {0,0,1}, ZUN[3] = { 0, 0,-1},
  ORG[3] = {0,0,0};
 
GLfloat viewangle = 0, tippangle = 0, traj[120][3];
 
GLfloat d[3] = {0.1, 0.1, 0.1};
 
GLfloat  xAngle = 0.0, yAngle = 0.0, zAngle = 0.0;


typedef struct tagBITMAPFILEHEADER
{
  uint16_t bfType;  //specifies the file type
  uint32_t bfSize;  //specifies the size in bytes of the bitmap file
  uint16_t bfReserved1;  //reserved; must be 0
  uint16_t bfReserved2;  //reserved; must be 0
  uint32_t bOffBits;  //species the offset in bytes from the bitmapfileheader to the bitmap bits
}BITMAPFILEHEADER;

typedef struct tagBITMAPINFOHEADER
{
  uint32_t biSize;  //specifies the number of bytes required by the struct
  long biWidth;  //specifies width in pixels
  long biHeight;  //species height in pixels
  uint16_t biPlanes; //specifies the number of color planes, must be 1
  uint16_t biBitCount; //specifies the number of bit per pixel
  uint32_t biCompression;//spcifies the type of compression
  uint32_t biSizeImage;  //size of image in bytes
  long biXPelsPerMeter;  //number of pixels per meter in x axis
  long biYPelsPerMeter;  //number of pixels per meter in y axis
  uint32_t biClrUsed;  //number of colors used by th ebitmap
  uint32_t biClrImportant;  //number of colors that are important
}BITMAPINFOHEADER;


 
float anglex = 35;
float angley = -35;
float anglez = 0;
float locZ = 0.0;
float locY = 0.0;
float locX = 0.0;
 
GLUquadric *earth;
GLuint earthTexture;
 
int LoadBitmap(char *filename)
{
  FILE * file;
  char temp;
  long i;
 
  BITMAPINFOHEADER infoheader;
  unsigned char *infoheader_data;
 
  GLuint num_texture;
 
  if( (file = fopen(filename, "rb"))==NULL) return (-1); // Open the file for reading
 
  //18
  fseek(file, 18, SEEK_CUR);  /* start reading width & height */
  fread(&infoheader.biWidth, sizeof(uint32_t), 1, file);
 
  fread(&infoheader.biHeight, sizeof(uint32_t), 1, file);
 
  fread(&infoheader.biPlanes, sizeof(uint16_t), 1, file);
  if (infoheader.biPlanes != 1) {
    printf("Planes from %s is not 1: %u\n", filename, infoheader.biPlanes);
    return 0;
  }
 
  // read the bpp
  fread(&infoheader.biBitCount, sizeof(unsigned short int), 1, file);
  if (infoheader.biBitCount != 24) {
    printf("Bpp from %s is not 24: %d\n", filename, infoheader.biBitCount);
    return 0;
  }
 
  fseek(file, 24, SEEK_CUR);
 
  // read the data



  printf("height: %lu", infoheader.biHeight);

  infoheader.biWidth = 512;

  if(infoheader.biWidth<0){
    infoheader.biWidth = -infoheader.biWidth;
  }
  if(infoheader.biHeight<0){
    infoheader.biHeight = -infoheader.biHeight;
  }


  //  infoheader_data = (uint8_t *) malloc(infoheader.biWidth * infoheader.biHeight * 3);
   infoheader_data = (uint8_t *) malloc(512 * infoheader.biHeight * 3);
  if (infoheader_data == NULL) {
    printf("Error allocating memory for color-corrected image data\n");
    return 0;
  }
 
  if ((i = fread(infoheader_data, infoheader.biWidth * infoheader.biHeight * 3, 1, file)) != 1) {
    printf("Error reading image data from %s.\n", filename);
    return 0;
  }
 
  /*  for (i=0; i<(infoheader.biWidth * infoheader.biHeight * 3); i+=3) { // reverse all of the colors. (bgr -> rgb)
    temp = infoheader_data[i];
    infoheader_data[i] = infoheader_data[i+2];
    infoheader_data[i+2] = temp;
    }*/
 
 
  fclose(file); // Closes the file stream
 
  glGenTextures(1, &num_texture);
  glBindTexture(GL_TEXTURE_2D, num_texture); // Bind the ID texture specified by the 2nd parameter
 
  // The next commands sets the texture parameters
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // If the u,v coordinates overflow the range 0,1 the image is repeated
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // The magnification function ("linear" produces better results)
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST); //The minifying function
 
  glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
 
  // Finally we define the 2d texture
  glTexImage2D(GL_TEXTURE_2D, 0, 3, infoheader.biWidth, infoheader.biHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, infoheader_data);
 
  // And create 2d mipmaps for the minifying function
  gluBuild2DMipmaps(GL_TEXTURE_2D, 3, infoheader.biWidth, infoheader.biHeight, GL_RGB, GL_UNSIGNED_BYTE, infoheader_data);
 
  free(infoheader_data); // Free the memory we used to load the texture
 
  return (num_texture); // Returns the current texture OpenGL ID
}
 
 
//---+----3----+----2----+----1----+---<>---+----1----+----2----+----3----+----4
 
//  Use arrow keys to rotate entire scene !!!
 
void Special_Keys (int key, int x, int y)
{
  switch (key) {
 
  case GLUT_KEY_LEFT :  viewangle -= 5;  break;
  case GLUT_KEY_RIGHT:  viewangle += 5;  break;
  case GLUT_KEY_UP   :  tippangle -= 5;  break;
  case GLUT_KEY_DOWN :  tippangle += 5;  break;
 
  default: printf ("   Special key %c == %d\n", key, key);
  }
 
  glutPostRedisplay();
}
 
//---+----3----+----2----+----1----+---<>---+----1----+----2----+----3----+----4
 
void Keyboard (unsigned char key, int x, int y)
{
  switch (key) {
 
  case 'j' : d[0] += 0.1;  break;
  case 'k' : d[1] += 0.1;  break;
  case 'l' : d[2] += 0.1;  break;
 
  case 'x' : xAngle += 5;  break;
  case 'y' : yAngle += 5;  break;
  case 'z' : zAngle += 5;  break;
 
  default: printf ("   Keyboard %c == %d\n", key, key);
  }
 
  glutPostRedisplay();
}
 
//---+----3----+----2----+----1----+---<>---+----1----+----2----+----3----+----4
 
void Triad (void)
{
  glColor3f (1.0, 1.0, 1.0);
 
  glBegin (GL_LINES);
  glVertex3fv (ORG); glVertex3fv (XUP);
  glVertex3fv (ORG); glVertex3fv (YUP);
  glVertex3fv (ORG); glVertex3fv (ZUP);
  glEnd ();
 
  glRasterPos3f (1.1, 0.0, 0.0);
  glutBitmapCharacter (GLUT_BITMAP_HELVETICA_18, 'X');
 
  glRasterPos3f (0.0, 1.1, 0.0);
  glutBitmapCharacter (GLUT_BITMAP_HELVETICA_18, 'Y');
 
  glRasterPos3f (0.0, 0.0, 1.1);
  glutBitmapCharacter (GLUT_BITMAP_HELVETICA_18, 'Z');
}
 
void redraw (void)
{
  int v;
 
  glClear  (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glEnable (GL_DEPTH_TEST);
  glEnable ( GL_TEXTURE_2D );
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity ();
 
  glTranslatef (0, 0, -3);
  glRotatef (tippangle, 1,0,0);  // Up and down arrow keys 'tip' view.
  glRotatef (viewangle, 0,1,0);  // Right/left arrow keys 'turn' view.
 
  glDisable (GL_LIGHTING);
 
  Triad ();

  //draw textured sphere
  glPushMatrix();
  glTranslatef(locX,locY,locZ);
  //glScalef(0.5,0.5,0.5);
  glRotatef (zAngle, 0,0,1);
  glRotatef (yAngle, 0,1,0);
  glRotatef (xAngle, 1,0,0);
  gluSphere( earth, 0.9, 36, 72);
  glPopMatrix();

  glDisable ( GL_TEXTURE_2D );
  glutSwapBuffers();
}
 
int main (int argc, char **argv)
{
  glutInit               (&argc, argv);
  glutInitWindowSize     (900, 600);
  glutInitWindowPosition (300, 300);
  glutInitDisplayMode    (GLUT_DEPTH | GLUT_DOUBLE);
 
  glutCreateWindow ("Orbital Font Demo");
  glutDisplayFunc  (   redraw   );
  glutKeyboardFunc (  Keyboard  );
  glutSpecialFunc  (Special_Keys);
 
  glClearColor (0.1, 0.0, 0.1, 1.0);


  earth = gluNewQuadric();
  gluQuadricTexture( earth, GL_TRUE);
  earthTexture = LoadBitmap("earth.bmp");

 
  glMatrixMode   (GL_PROJECTION);
  gluPerspective (60, 1.5, 1, 10);
  glMatrixMode   (GL_MODELVIEW);
  glutMainLoop   ();
 
  return 1;
}
 
//---+----3----+----2----+----1----+---<>---+----1----+----2----+----3----+----4
