// main.cpp

#define GLEW_STATIC
#include <GL/glew.h>
#include <GL/freeglut.h>

#include "shader.h"
#include "texture.h"
#include "vec2.h"
#include "vec3.h"

#define BUFFER_OFFSET(bytes) ((GLubyte*)nullptr + (bytes))

static int width = 800, height = 600;

static Shader *shader = nullptr;
static GLuint vao, vbo;
static GLuint texture_diffuse;
static GLuint texture_normal;
static GLuint texture_height;

vec3 light_pos(0.5f, 1.0f, 0.3f);

// Camera
//Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

GLboolean parallax_mapping = true;
GLfloat height_scale = 0.1;

/*==========================================================================*\
 * init_quad                                                                *
\*==========================================================================*/
static void init_quad()
{
  // positions
  vec3 pos1(-1.0, 1.0, 0.0);
  vec3 pos2(-1.0, -1.0, 0.0);
  vec3 pos3(1.0, -1.0, 0.0);
  vec3 pos4(1.0, 1.0, 0.0);
  // texture coordinates
  vec2 uv1(0.0, 1.0);
  vec2 uv2(0.0, 0.0);
  vec2 uv3(1.0, 0.0);
  vec2 uv4(1.0, 1.0);
  // normal vector
  vec3 nm(0.0, 0.0, 1.0);

  // calculate tangent/bitangent vectors of both triangles
  vec3 tangent1, bitangent1;
  vec3 tangent2, bitangent2;
  // - triangle 1
  vec3 edge1 = pos2 - pos1;
  vec3 edge2 = pos3 - pos1;
  vec2 deltaUV1 = uv2 - uv1;
  vec2 deltaUV2 = uv3 - uv1;

  GLfloat f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

  tangent1.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
  tangent1.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
  tangent1.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
  tangent1.normalize();

  bitangent1.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
  bitangent1.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
  bitangent1.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);
  bitangent1.normalize();

  // - triangle 2
  edge1 = pos3 - pos1;
  edge2 = pos4 - pos1;
  deltaUV1 = uv3 - uv1;
  deltaUV2 = uv4 - uv1;

  f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

  tangent2.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
  tangent2.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
  tangent2.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
  tangent2.normalize();


  bitangent2.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
  bitangent2.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
  bitangent2.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);
  bitangent2.normalize();


  GLfloat vertices[] =
  {
    // Positions            // normal         // TexCoords  // Tangent                          // Bitangent
    pos1.x, pos1.y, pos1.z, nm.x, nm.y, nm.z, uv1.x, uv1.y, tangent1.x, tangent1.y, tangent1.z, bitangent1.x, bitangent1.y, bitangent1.z,
    pos2.x, pos2.y, pos2.z, nm.x, nm.y, nm.z, uv2.x, uv2.y, tangent1.x, tangent1.y, tangent1.z, bitangent1.x, bitangent1.y, bitangent1.z,
    pos3.x, pos3.y, pos3.z, nm.x, nm.y, nm.z, uv3.x, uv3.y, tangent1.x, tangent1.y, tangent1.z, bitangent1.x, bitangent1.y, bitangent1.z,

    pos1.x, pos1.y, pos1.z, nm.x, nm.y, nm.z, uv1.x, uv1.y, tangent2.x, tangent2.y, tangent2.z, bitangent2.x, bitangent2.y, bitangent2.z,
    pos3.x, pos3.y, pos3.z, nm.x, nm.y, nm.z, uv3.x, uv3.y, tangent2.x, tangent2.y, tangent2.z, bitangent2.x, bitangent2.y, bitangent2.z,
    pos4.x, pos4.y, pos4.z, nm.x, nm.y, nm.z, uv4.x, uv4.y, tangent2.x, tangent2.y, tangent2.z, bitangent2.x, bitangent2.y, bitangent2.z
  };
  // Setup plane VAO
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof vertices, &vertices, GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(GLfloat), (GLvoid*)0);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
  glEnableVertexAttribArray(2);
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 14 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
  glEnableVertexAttribArray(3);
  glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(GLfloat), (GLvoid*)(8 * sizeof(GLfloat)));
  glEnableVertexAttribArray(4);
  glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(GLfloat), (GLvoid*)(11 * sizeof(GLfloat)));
}

/*==========================================================================*\
 * keyboard_callback                                                        *
\*==========================================================================*/
static void keyboard_callback(unsigned char key, int, int)
{
  switch (key)
  {
    case 'E':
      height_scale -= 0.001;
      break;
    case 'e':
      height_scale += 0.001;
      break;
    case ' ':
      parallax_mapping = !parallax_mapping;
      break;
    case 'q': case 'Q': case 27: exit(0);
  }
  glutPostRedisplay();
}

/*==========================================================================*\
 * special_callback                                                         *
\*==========================================================================*/
static void special_callback(int key, int, int)
{
  switch (key)
  {
    case GLUT_KEY_UP:
      break;
    case GLUT_KEY_DOWN:
      break;
    case GLUT_KEY_LEFT:
      break;
    case GLUT_KEY_RIGHT:
      break;
  }
  glutPostRedisplay();
}

/*==========================================================================*\
 * timer_callback                                                           *
\*==========================================================================*/
static void timer_callback(int)
{
  glutPostRedisplay();
  glutTimerFunc(25, timer_callback, 0);
}

/*==========================================================================*\
 * display_callback                                                         *
\*==========================================================================*/
static void display_callback()
{
  vec3 eye(0, 0, 3);

  // Clear the colorbuffer
  glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Configure view/projection matrices
  matrix projection;
  projection.set_perspective(45, static_cast<GLfloat>(width) / height, 0.1, 100.0);
  matrix view;
  view.set_look_at(eye, vec3(0,0,0), vec3(0,1,0));
  matrix model;
  model.rotate(-30, vec3(1,0,0));
  model.rotate(glutGet(GLUT_ELAPSED_TIME) / 100.0, vec3(0, 0, 1)); // Rotates the quad to show parallax mapping works in all directions

  // update the uniforms
  shader->activate();
  shader->set("view", view);
  shader->set("projection", projection);
  shader->set("model", model);
  shader->set("light_pos", light_pos);
  shader->set("view_pos", eye);
//  shader->set("height_scale", height_scale);
//  shader->set("parallax", parallax_mapping);

  // Affectation des textures
  glActiveTexture(GL_TEXTURE0); glBindTexture(GL_TEXTURE_2D, texture_diffuse);
  glActiveTexture(GL_TEXTURE1); glBindTexture(GL_TEXTURE_2D, texture_normal);
  glActiveTexture(GL_TEXTURE2); glBindTexture(GL_TEXTURE_2D, texture_height);
  glBindVertexArray(vao);
  glDrawArrays(GL_TRIANGLES, 0, 6);
  glBindVertexArray(0);

  glutSwapBuffers();
}

/*==========================================================================*\
 * init                                                                     *
\*==========================================================================*/
static void init()
{
  // Load textures
  texture_diffuse = get_texture("data/brick_diffuse.ppm");
  texture_normal  = get_texture("data/brick_normal.ppm");
  texture_height  = get_texture("data/brick_height.ppm");

  // Setup and compile our shaders
  shader = new Shader();
  shader->attach_vertex_from_file("data/parallax.vert");
  shader->attach_fragment_from_file("data/parallax.frag");
  shader->set_attributes({"position", "normal", "tex_coords", "tangent", "bitangent"});
  shader->link();

  // Set texture units 
  shader->activate();
  shader->set("diffuse_tex", 0);
  shader->set("normal_tex", 1);
  // shader->set("height_tex", 2); // pour le parallax

  init_quad();
  glEnable(GL_DEPTH_TEST);
}

int main(int argc, char *argv[])
{
  glutInitContextVersion(3, 3);
  glutInitContextFlags(GLUT_FORWARD_COMPATIBLE | GLUT_DEBUG);
  glutInitContextProfile(GLUT_CORE_PROFILE);

  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH | GLUT_STENCIL);
  glutInitWindowSize(width, height);
  glutCreateWindow("Parallax mapping");
  glutDisplayFunc(display_callback);
  glutKeyboardFunc(keyboard_callback);
  glutSpecialFunc(special_callback);

  glutTimerFunc(25, timer_callback, 0);
  glewExperimental = true; glewInit();glGetError();

  init();

  glutMainLoop();
}

