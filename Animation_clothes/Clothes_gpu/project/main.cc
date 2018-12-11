// main.cc

#define GLEW_STATIC 1
#include <GL/glew.h>
#include <GL/glut.h>

#include "image.h"
#include "gl-utils.h"
#include "vec2.h"
#include "vec4.h"

//#include "mathlib.h"

const unsigned SIZE = 128;

// Programs.
static GLuint program_positions;
static GLuint program_speeds;
static GLuint program_normals;

// VBOs.
static GLuint vbo_vertex;
static GLuint vbo_normal;
static GLuint vbo_tex_coord;
static GLuint vbo_index;

// Textures & FBO.
static GLuint texture_positions[2];
static GLuint texture_speeds[2];
static GLuint texture_normals[2];
static GLuint fbo_positions[2];
static GLuint fbo_speeds[2];
static GLuint fbo_normals[2];

static int current = 0;

static GLuint texture_img;

static int width = 1024;
static int height = 768;

static bool goon;
static bool wireframe = false;
static float angle = -54.0;
static float angle2 = 26.0;

static float attached = 0.1;

static void
init (void)
{
  goon = true;

  Image *image = image_load_tga ("data/texture.tga");
  if (image)
    {
      glPixelStorei (GL_UNPACK_ALIGNMENT, 1);
      glGenTextures (1, &texture_img);
      glBindTexture (GL_TEXTURE_2D, texture_img);
      glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
      glTexImage2D (GL_TEXTURE_2D, 0, GL_RGB, image->width, image->height, 0, GL_RGB, GL_UNSIGNED_BYTE, image->data);
      delete image;
    }
  else
    abort ();

  glGenBuffers (1, &vbo_vertex);
  glBindBuffer (GL_ARRAY_BUFFER, vbo_vertex);
  glBufferData (GL_ARRAY_BUFFER, SIZE * SIZE * 4 * sizeof (GLfloat), 0, GL_DYNAMIC_COPY);
  glVertexPointer (3, GL_FLOAT, 16, 0);

  glGenBuffers (1, &vbo_normal);
  glBindBuffer (GL_ARRAY_BUFFER, vbo_normal);
  glBufferData (GL_ARRAY_BUFFER, SIZE * SIZE * 4 * sizeof (GLfloat), 0, GL_DYNAMIC_COPY);
  glNormalPointer (GL_FLOAT, 16, 0);

  // Calculate texcoords buffer data.
  vec2 tex_coords[SIZE * SIZE];
  for (unsigned j = 0; j < SIZE; j++)
      for (unsigned i = 0; i < SIZE; i++)
          tex_coords[i + j * SIZE] = vec2 ((i + 0.5) / SIZE, (j + 0.5) / SIZE);
  glGenBuffers (1, &vbo_tex_coord);
  glBindBuffer (GL_ARRAY_BUFFER, vbo_tex_coord);
  glBufferData (GL_ARRAY_BUFFER, sizeof tex_coords, tex_coords, GL_STATIC_DRAW);
  glTexCoordPointer (2, GL_FLOAT, 0, 0);

  // Calculate indices buffer data.
  unsigned indices[SIZE * (SIZE - 1) * 2];
  for (unsigned j = 0; j < SIZE - 1; j++)
      for (unsigned i = 0; i < SIZE; i++)
          if (j & 1) {
              indices[j*SIZE*2+2*i+1] = SIZE-1-i      +j*SIZE;
              indices[j*SIZE*2+2*i]   = SIZE-1-i+SIZE +j*SIZE;
          } else {
              indices[j*SIZE*2+2*i+1] = i+SIZE +j*SIZE;
              indices[j*SIZE*2+2*i]   = i      +j*SIZE;
          }
  glGenBuffers (1, &vbo_index);
  glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, vbo_index);
  glBufferData (GL_ELEMENT_ARRAY_BUFFER, sizeof indices, indices, GL_STATIC_DRAW);

  // Calculate positions texture data.
  vec4 data[SIZE * SIZE];
  for (unsigned j = 0; j < SIZE; j++)
      for (unsigned i = 0; i < SIZE; i++)
          data[i + j * SIZE] = vec4 (4.0 * i / (SIZE - 1) - 2.0, 1.0, 4.0 * j / (SIZE - 1) - 2.0, 0.0);
  glGenFramebuffersEXT (2, fbo_positions); PRINT_OPENGL_ERROR ();
  glGenTextures (2, texture_positions); PRINT_OPENGL_ERROR ();
  for (unsigned i = 0; i < 2; i++)
    {
      glBindFramebufferEXT (GL_FRAMEBUFFER_EXT, fbo_positions[i]);
      glBindTexture (GL_TEXTURE_2D, texture_positions[i]);
      glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
      glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
      glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
      glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
      glTexImage2D (GL_TEXTURE_2D, 0, GL_RGBA32F_ARB, SIZE, SIZE, 0, GL_RGBA, GL_FLOAT, data);
      glFramebufferTexture2DEXT (GL_FRAMEBUFFER_EXT,
                                 GL_COLOR_ATTACHMENT0_EXT,
                                 GL_TEXTURE_2D, texture_positions[i], 0); PRINT_OPENGL_ERROR ();
      check_framebuffer_status ();
    }

  // Calculate speeds texture data.
  for (unsigned j = 0; j < SIZE; j++)
      for (unsigned i = 0; i < SIZE; i++)
          data[i + j * SIZE] = vec4 (0.0, 0.0, 0.0, 0.0);
  glGenFramebuffersEXT (2, fbo_speeds); PRINT_OPENGL_ERROR ();
  glGenTextures (2, texture_speeds); PRINT_OPENGL_ERROR ();
  for (unsigned i = 0; i < 2; i ++)
    {
      glBindFramebufferEXT (GL_FRAMEBUFFER_EXT, fbo_speeds[i]);
      glBindTexture (GL_TEXTURE_2D, texture_speeds[i]);
      glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
      glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
      glTexImage2D (GL_TEXTURE_2D, 0, GL_RGBA32F_ARB, SIZE, SIZE, 0, GL_RGBA, GL_FLOAT, data);
      glFramebufferTexture2DEXT (GL_FRAMEBUFFER_EXT,
          GL_COLOR_ATTACHMENT0_EXT,
          GL_TEXTURE_2D, texture_speeds[i], 0); PRINT_OPENGL_ERROR ();
      check_framebuffer_status ();
    }

  // Calculate normals texture data.
  for (unsigned j = 0; j < SIZE; j++)
      for (unsigned i = 0; i < SIZE; i++)
          data[i + j * SIZE] = vec4 (0.0, 1.0, 0.0, 0.0);
  glGenFramebuffersEXT (2, fbo_normals); PRINT_OPENGL_ERROR ();
  glGenTextures (2, texture_normals); PRINT_OPENGL_ERROR ();
  for (unsigned i = 0; i < 2; i++)
    {
      glBindFramebufferEXT (GL_FRAMEBUFFER_EXT, fbo_normals[i]);
      glBindTexture (GL_TEXTURE_2D, texture_normals[i]);
      glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
      glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
      glTexImage2D (GL_TEXTURE_2D, 0, GL_RGBA32F_ARB, SIZE, SIZE, 0, GL_RGBA, GL_FLOAT, data);
      glFramebufferTexture2DEXT (GL_FRAMEBUFFER_EXT,
          GL_COLOR_ATTACHMENT0_EXT,
          GL_TEXTURE_2D, texture_normals[i], 0); PRINT_OPENGL_ERROR ();
      check_framebuffer_status ();
    }
  glBindFramebufferEXT (GL_FRAMEBUFFER_EXT, 0);

  // Loading shaders.
  program_speeds = read_shader ("", "data/speeds.frag");
  glUniform1i (get_uni_loc (program_speeds, "positions"), 0);
  glUniform1i (get_uni_loc (program_speeds, "speeds"), 1);
  glUniform1i (get_uni_loc (program_speeds, "normals"), 2);

  program_positions = read_shader ("", "data/positions.frag");
  glUniform1i (get_uni_loc (program_positions, "positions"), 0);
  glUniform1i (get_uni_loc (program_positions, "speeds"), 1);

  program_normals = read_shader ("", "data/normals.frag");
  glUniform1i (get_uni_loc (program_normals, "positions"), 0);

  glUseProgram (0);

  glClearColor (0.2, 0.3, 0.4, 0);
  glEnableClientState (GL_VERTEX_ARRAY);
  glEnableClientState (GL_NORMAL_ARRAY);
  glEnableClientState (GL_TEXTURE_COORD_ARRAY);
}

static void
draw_full_quad (void)
{
  glBegin (GL_QUADS);
    {
      glTexCoord2f (attached, 0); glVertex2f (attached, 0);
      glTexCoord2f (attached, 1); glVertex2f (attached, 1);
      glTexCoord2f (1, 1); glVertex2f (1, 1);
      glTexCoord2f (1, 0); glVertex2f (1, 0);
    }
  glEnd ();
}

static void
update_fbos (void)
{
  glViewport (0,0, SIZE, SIZE);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(0,1,0,1,-1,1);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  glActiveTexture(GL_TEXTURE0);
  glBindTexture (GL_TEXTURE_2D, texture_positions[1 - current]);
  glActiveTexture(GL_TEXTURE1);
  glBindTexture (GL_TEXTURE_2D, texture_speeds[1 - current]);
  glActiveTexture(GL_TEXTURE2);
  glBindTexture (GL_TEXTURE_2D, texture_normals[1 - current]);

  // Updating speeds.
  glBindFramebufferEXT (GL_FRAMEBUFFER_EXT, fbo_speeds[current]);
  glUseProgram (program_speeds);
  draw_full_quad ();
  glActiveTexture(GL_TEXTURE1);
  glBindTexture (GL_TEXTURE_2D, texture_speeds[current]);

  // Updading positions.
  glBindFramebufferEXT (GL_FRAMEBUFFER_EXT, fbo_positions[current]);
  glUseProgram (program_positions);
  draw_full_quad ();
  glBindBuffer (GL_PIXEL_PACK_BUFFER, vbo_vertex);
  glReadPixels(0,0,SIZE,SIZE,GL_RGBA,GL_FLOAT,0);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture (GL_TEXTURE_2D, texture_positions[current]);

  // Updading normals.
  glBindFramebufferEXT (GL_FRAMEBUFFER_EXT, fbo_normals[current]);
  glUseProgram (program_normals);
  draw_full_quad ();
  glBindBuffer (GL_PIXEL_PACK_BUFFER, vbo_normal);
  glReadPixels(0,0,SIZE,SIZE,GL_RGBA,GL_FLOAT,0);

  glBindFramebufferEXT (GL_FRAMEBUFFER_EXT, 0);
  glUseProgram (0);
  glActiveTexture(GL_TEXTURE0);
}

static
void display_callback ()
{
  if (goon)
      update_fbos ();

  glViewport (0,0, width, height);
  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glEnable (GL_DEPTH_TEST);
  glMatrixMode (GL_PROJECTION);
  glLoadIdentity ();
  gluPerspective (45, float (width) / height, 1, 10);
  glMatrixMode (GL_MODELVIEW);
  glLoadIdentity ();
  gluLookAt (0, 0.5, 5, 0,0, -1, 0,1,0);

  glEnable (GL_TEXTURE_2D);
  glBindTexture (GL_TEXTURE_2D, texture_img);

  GLfloat lpos[4] = { 0, 2.5, 5, 0.0};
  glLightfv (GL_LIGHT0, GL_POSITION, lpos);
  glEnable (GL_LIGHTING);
  glEnable (GL_LIGHT0);
  glEnable (GL_COLOR_MATERIAL);
  if (wireframe)
    glPolygonMode (GL_FRONT_AND_BACK, GL_LINE);
  glRotatef (angle2, 1,0,0);
  glRotatef (angle, 0,1,0);

  glDrawElements (GL_TRIANGLE_STRIP, SIZE * (SIZE - 1) * 2, GL_UNSIGNED_INT, 0);
//const vec2 cylinder = vec2 (1.0, 0.0);
//const float cylinder_radius = 0.4;
  glDisable (GL_TEXTURE_2D);
  glPushMatrix ();
  glTranslatef (-0.7, 0.0, -0.8);
  glColor3f (1,1,0);
  glutSolidSphere (0.69, 64, 64);
  glPopMatrix ();

  glPushMatrix ();
  glTranslatef (-0.4, 0.0, 0.5);
  glColor3f (0,1,1);
  glutSolidSphere (0.59, 64, 64);
  glPopMatrix ();

  GLUquadric *cylinder = gluNewQuadric ();
  glTranslatef (1.0, 0.0, -2.5);
  glColor3f (1,0,1);
  gluCylinder (cylinder, 0.39, 0.39, 5, 64, 3);
  gluDeleteQuadric (cylinder);

  glColor3f (1,1,1);
  glDisable (GL_DEPTH_TEST);
  glDisable (GL_LIGHTING);

  PRINT_OPENGL_ERROR ();

  glPolygonMode (GL_FRONT_AND_BACK, GL_FILL);
  current = 1 - current;
  glutSwapBuffers ();
}

static void
reshape_callback (int w, int h)
{
  width  = w;
  height = h;

  glViewport (0,0, width, height); PRINT_OPENGL_ERROR ();
  glutPostRedisplay ();
}

static void
keyboard_callback (unsigned char key, int, int)
{
  switch (key)
    {
    case 'a':
      attached = std::max (0.0f, attached - 0.01f);
      break;
    case 'A':
      attached = std::min (1.0f, attached + 0.01f);
      break;
    case 'q':
    case 'Q':
    case 27:
      exit (0);
    case 'w':
    case 'W':
      wireframe = !wireframe;
    case ' ':
      goon = !goon;
      break;
    }
  glutPostRedisplay ();
}

static void
special_callback (int key, int, int)
{
  switch (key)
    {
    case GLUT_KEY_LEFT:
      angle += 2.0;
      break;
    case GLUT_KEY_RIGHT:
      angle -= 2.0;
      break;
    case GLUT_KEY_UP:
      angle2 += 2.0;
      break;
    case GLUT_KEY_DOWN:
      angle2 -= 2.0;
      break;
    }
  glutPostRedisplay ();
}

static void
idle_callback ()
{
  glutPostRedisplay ();
}

int
main (int argc, char **argv)
{
  glutInit (&argc, argv);
  glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowSize (width, height);
  glutCreateWindow ("Mass-spring");
  glutDisplayFunc (display_callback);
  glutReshapeFunc (reshape_callback);
  glutKeyboardFunc (keyboard_callback);
  glutSpecialFunc (special_callback);
  glutIdleFunc (idle_callback);

  glewInit ();

  init ();

  glutMainLoop ();
}
