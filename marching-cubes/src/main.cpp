// Fichier principal de l'implémentation de l'algorithme des marching cubes

#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <string>
#include <vector>
#include <cmath>

#define GLEW_STATIC 1
#include <GL/glew.h>
#include <GL/freeglut.h>

#include "shader.h"
#include "gl-utils.h"
#include "image.h"

#include "marching_cubes.h"

#define NB_BLOCKS_X 3
#define NB_BLOCKS_Y 3
#define NB_BLOCKS_Z 8
#define NB_BLOCKS (NB_BLOCKS_X * NB_BLOCKS_Y * NB_BLOCKS_Z)

//FIXME! put this in a struct or something.
int win_width = 800, win_height = 600;
int win_zoom = 45;
int angle_y = 0;
int alt_y = 0;
int pos_x = 300;

GLuint lt_pos_vbo;
GLuint lt_edge_vbo;

struct VBlock
{
  GLuint  vertices_vbo;
  GLuint  normals_vbo;
  GLint   nb_vertices;
  GLfloat x, y, z;
};

VBlock blocks[NB_BLOCKS];

GLuint cluster_vbo;

GLuint surfacetex;

// for the density pass
static Shader *shader_density = nullptr;
static GLuint fb, density_tex;
static GLuint density_vbo, density_vao;

// list triangle pass
static GLuint list_triangle_vao;
static Shader *shader_lt = nullptr;
// generate vertices
static GLuint generate_vertices_vao;
static Shader *shader_gv = nullptr;

// Render pass
static Shader *shader_render = nullptr;
static GLuint render_vao;

/*===========================================================================*\
 * z_to_offset                                                               *
\*===========================================================================*/
static void z_to_offset(int z, int xy, GLfloat offset[3])
{
  offset[0] = ((xy % 3) - 1) * 2;
  offset[1] = ((xy / 3) - 1) * 2;
  offset[2] = (z - 3.5) * 2; // -7, -5, -3, -1, 1, 3, 5, 7
}

/*===========================================================================*\
 * compute_density_pass_display                                              *
\*===========================================================================*/
static void compute_density_pass_display(int z, int xy, bool doit)
{
  GLfloat offset[3];

  glBindVertexArray(density_vao);
  glBindFramebuffer(GL_FRAMEBUFFER, fb);
  glViewport(0, 0, 48, 48);
  glDisable(GL_DEPTH_TEST);
  shader_density->activate();
  z_to_offset(z, xy, offset);
  shader_density->set("offset", offset[0], offset[1], offset[2]);
  if (doit)
    glDrawArraysInstanced(GL_TRIANGLE_FAN, 0, 4, 48); // mode, first, count, prim count, so 48 quads
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glBindVertexArray(0);
}

/*===========================================================================*\
 * build_block_passes_display                                                *
\*===========================================================================*/
static void build_block_passes_display(int z, int xy)
{
  GLfloat offset[3];
  // list triangle pass
  shader_lt->activate();
  glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, lt_pos_vbo);
  glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 1, lt_edge_vbo);
  glEnable(GL_RASTERIZER_DISCARD);
  glBindVertexArray(list_triangle_vao);
  glBeginTransformFeedback(GL_POINTS);
  GLuint queryId;
  glGenQueries(1, &queryId);
  glBeginQuery(GL_PRIMITIVES_GENERATED, queryId);
  glBindBuffer(GL_ARRAY_BUFFER, cluster_vbo);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
  glDrawArrays(GL_POINTS, 0, 32 * 32 * 32);
  glEndQuery(GL_PRIMITIVES_GENERATED);
  glGetQueryObjectiv(queryId, GL_QUERY_RESULT, &blocks[z * 9 + xy].nb_vertices);
  std::cout << "nb vertices: " << blocks[z * 9 + xy].nb_vertices << std::endl;
  glDeleteQueries(1, &queryId);
  glEndTransformFeedback();
  glBindVertexArray(0);
  glDisable(GL_RASTERIZER_DISCARD);

  if (blocks[z * 9 + xy].nb_vertices)
  {
    // Generate vertices
    shader_gv->activate();
    glEnable(GL_RASTERIZER_DISCARD);
    glBindVertexArray(generate_vertices_vao);
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_3D, density_tex);
    glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, blocks[z * 9 + xy].vertices_vbo);
    glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 1, blocks[z * 9 + xy].normals_vbo);
    glBeginTransformFeedback(GL_TRIANGLES);
    glBindBuffer(GL_ARRAY_BUFFER, lt_pos_vbo);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    glBindBuffer(GL_ARRAY_BUFFER, lt_edge_vbo);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    z_to_offset(z, xy, offset);
    shader_gv->set("offset", offset[0], offset[1], offset[2]);
    glDrawArrays(GL_POINTS, 0, blocks[z * 9 + xy].nb_vertices);
    glEndTransformFeedback();

    glBindVertexArray(0);
    glDisable(GL_RASTERIZER_DISCARD);
  }
}

/*===========================================================================*\
 * render_rock_pass_display                                                  *
\*===========================================================================*/
static void render_rock_pass_display()
{
  matrix projection, modelview;
  glViewport(0, 0, win_width, win_height);
  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
  shader_render->activate();
  projection.set_perspective(win_zoom, static_cast<float>(win_width)/win_height, 0.1, 100);
  projection.translate(vec3(0, alt_y / 25.0, -20+pos_x / 25.0));
  shader_render->set("projection", projection);

  modelview.rotate(angle_y, vec3(0,1,0));
  modelview.rotate(-90, vec3(1,0,0));
  shader_render->set("modelview", modelview);

  glEnable(GL_DEPTH_TEST);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, surfacetex);

  glBindVertexArray(render_vao);

  for (int i = 0; i < NB_BLOCKS; i++)
  {
    if (blocks[i].nb_vertices)
    {
      glBindBuffer(GL_ARRAY_BUFFER, blocks[i].vertices_vbo);
      glEnableVertexAttribArray(0);
      glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, nullptr);
      glBindBuffer(GL_ARRAY_BUFFER, blocks[i].normals_vbo);
      glEnableVertexAttribArray(1);
      glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
      glDrawArrays(GL_TRIANGLES, 0, blocks[i].nb_vertices * 3);
    }
  }
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
  glDisable(GL_DEPTH_TEST);
}

/*===========================================================================*\
 * display                                                                   *
\*===========================================================================*/
static void display()
{
  static bool MC_done = false;
  static int  pass = 0;
  static int  xy   = 0;

  if (!MC_done)
  {
    compute_density_pass_display(pass, xy, true);
    build_block_passes_display(pass, xy);
    xy++;
    if (xy == 9)
    {
      std::cout << std::endl;
      xy = 0;
      pass++;
      if (pass == NB_BLOCKS_Z)
      {
        std::cout << "done" << std::endl;
        MC_done = true;
      }
    }
  }
  render_rock_pass_display();
  glViewport(0, 0, win_width, win_height);
  glutSwapBuffers();
}

/*===========================================================================*\
 * print_stats                                                               *
\*===========================================================================*/
static void print_stats()
{
  GLint temp;
  glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &temp);
  std::cout << "Max combined texture image units: " << temp << std::endl;
  glGetIntegerv(GL_MAX_TEXTURE_COORDS, &temp);
  std::cout << "Max texture coords: " << temp << std::endl;
  glGetIntegerv(GL_MAX_TEXTURE_UNITS, &temp);
  std::cout << "Max texture units: " << temp << std::endl;
  glGetIntegerv(GL_MAX_GEOMETRY_TEXTURE_IMAGE_UNITS_EXT, &temp);
  std::cout << "Max GS texture image units: " << temp << std::endl;
  glGetIntegerv(GL_MAX_GEOMETRY_OUTPUT_VERTICES_EXT, &temp);
  std::cout << "Max GS output vertices: " << temp << std::endl;
  glGetIntegerv(GL_MAX_GEOMETRY_TOTAL_OUTPUT_COMPONENTS_EXT, &temp);
  std::cout << "Max GS total output components: " << temp << std::endl;
  glGetIntegerv(GL_MAX_GEOMETRY_UNIFORM_COMPONENTS_EXT, &temp);
  std::cout << "Max GS uniform components: " << temp << std::endl;
  glGetIntegerv(GL_MAX_GEOMETRY_VARYING_COMPONENTS_EXT, &temp);
  std::cout << "Max GS varying components: " << temp << std::endl;
  glGetIntegerv(GL_MAX_VERTEX_VARYING_COMPONENTS_EXT, &temp);
  std::cout << "Max Vertex varying components: " << temp << std::endl;
  glGetIntegerv(GL_MAX_VARYING_COMPONENTS_EXT, &temp);
  std::cout << "Max Varying components: " << temp << std::endl;
  glGetIntegerv(0x8DE2 /*GL_MAX_BINDABLE_VERTEX_UNIFORMS_EXT*/, &temp);
  std::cout << "Max Bindable vertex uniforms: " << temp << std::endl;
  glGetIntegerv(0x8DE3 /*GL_MAX_BINDABLE_FRAGMENT_UNIFORMS_EXT*/, &temp);
  std::cout << "Max Bindable fragment uniforms: " << temp << std::endl;
  glGetIntegerv(0x8DE3 /*GL_MAX_BINDABLE_GEOMETRY_UNIFORMS_EXT*/, &temp);
  std::cout << "Max Bindable geometry uniforms: " << temp << std::endl;
  glGetIntegerv(GL_MAX_BINDABLE_UNIFORM_SIZE_EXT, &temp);
  std::cout << "Max Bindable uniform size uniforms: " << temp << std::endl;
  glGetIntegerv(GL_MAX_DRAW_BUFFERS_ATI, &temp);
  std::cout << "Max draw buffer ATI: " << temp << std::endl;
  glGetIntegerv(GL_MAX_TRANSFORM_FEEDBACK_SEPARATE_ATTRIBS_NV, &temp);
  std::cout << "Max transform feedback separate attribs: " << temp << std::endl;
  glGetIntegerv(GL_MAX_TRANSFORM_FEEDBACK_INTERLEAVED_COMPONENTS_NV, &temp);
  std::cout << "Max transform feedback interleaved components: " << temp << std::endl;
  glGetError(); // FIXME, this is just to shut up a warning
}

/*===========================================================================*\
 * create_build_density_shader                                               *
\*===========================================================================*/
static Shader *create_build_density_shader()
{
  Shader *shader;
  /* build density shader */
  std::cout << "build density shader" << std::endl;
  shader = new Shader();
  shader->attach_fragment_from_file("shaders/build_density.frag");
  shader->attach_vertex_from_file  ("shaders/build_density.vert");
  shader->attach_geometry_from_file("shaders/build_density.geom");
  shader->set_attributes({"v_position"});
  shader->link();
  shader->activate();
  shader->set("noiseVol0", 4);
  shader->set("noiseVol1", 5);
  shader->set("noiseVol2", 6);
  shader->set("noiseVol3", 7);

  return shader;
}

/*===========================================================================*\
 * create_list_triangles_shader                                              *
\*===========================================================================*/
static Shader *create_list_triangles_shader()
{
  Shader *shader;
  /* list triangles shader */
  std::cout << "list triangles shader" << std::endl;
//  const char *targets[] = { "outtripos", "outtriedge", 0 };
  shader = new Shader();
  shader->attach_fragment_from_file("shaders/list_triangles.frag");
  shader->attach_vertex_from_file  ("shaders/list_triangles.vert");
  shader->attach_geometry_from_file("shaders/list_triangles.geom");//, GL_POINTS, GL_POINTS, 5);
  shader->set_attributes({"position"});
//  shader->set_targets (targets);
  std::vector<std::string> target;
  target.push_back("outtripos");
  target.push_back("outtriedge");
  shader->set_tf_targets(target);
  shader->link();
  shader->activate();

  shader->set("dataFieldTex", 3);
  shader->set("nb_tri_tex", 1);
  shader->set("triTableTex", 2);
  std::vector<GLfloat> cluster;
  for (int k = 0; k < 32; k++)
    for (int j = 0; j < 32 ; j++)
      for (int i = 0; i < 32 ; i++)
        {
          cluster.push_back(i);
          cluster.push_back(j);
          cluster.push_back(k);
        }
  glGenBuffers(1, &cluster_vbo); PRINT_OPENGL_ERROR();
  glBindBuffer(GL_ARRAY_BUFFER, cluster_vbo); PRINT_OPENGL_ERROR();
  glBufferData(GL_ARRAY_BUFFER, 32 * 32 * 32 * 3 * sizeof(GLfloat), &cluster[0], GL_STREAM_COPY); PRINT_OPENGL_ERROR();
  glBindBuffer(GL_ARRAY_BUFFER, 0); PRINT_OPENGL_ERROR();

  return shader;
}

/*===========================================================================*\
 * create_gen_vertices_shader                                                *
\*===========================================================================*/
static Shader *create_gen_vertices_shader()
{
  Shader *shader;
  /* gen vertices shader */
  std::cout << "gen vertices shader" << std::endl;
//  const char *targets[] = { "outvert", "outnorm", 0 };
  shader = new Shader();
  shader->attach_fragment_from_file("shaders/gen_vertices.frag");
  shader->attach_vertex_from_file  ("shaders/gen_vertices.vert");
  shader->attach_geometry_from_file("shaders/gen_vertices.geom");//, GL_POINTS, GL_TRIANGLE_STRIP, 3);
  shader->set_attributes({"position", "normal"});
  std::vector<std::string> target;
  target.push_back("outvert");
  target.push_back("outnorm");
  shader->set_tf_targets(target);
  //shader->set_targets(targets);
  shader->link();
  shader->activate();

  shader->set("dataFieldTex", 3);
  shader->set("poisson", 7);

  return shader;
}

/*===========================================================================*\
 * create_render_shader                                                      *
\*===========================================================================*/
static Shader *create_render_shader()
{
  Shader *shader;
  /* render shader */
  std::cout << "render shader" << std::endl;
  shader = new Shader();
  shader->attach_fragment_from_file("shaders/render.frag");
  shader->attach_vertex_from_file("shaders/render.vert");
  shader->set_attributes({"position", "normal"});
  shader->link();
  shader->activate();
  shader->set("myTexture", 0);
  shader->set("dataFieldTex", 3);
  shader->set("noise", 4);

  return shader;
}

/*===========================================================================*\
 * create_shader                                                             *
\*===========================================================================*/
static void create_shader()
{
  shader_density = create_build_density_shader();
  shader_lt      = create_list_triangles_shader();
  shader_gv      = create_gen_vertices_shader();
  shader_render  = create_render_shader();

  std::cout << "shaders created" << std::endl;
}

/*===========================================================================*\
 * reshape                                                                   *
\*===========================================================================*/
static void reshape(int w, int h)
{
  win_width  = w;
  win_height = h;
  glViewport(0, 0, w, h); PRINT_OPENGL_ERROR();
}

/*===========================================================================*\
 * keyboard                                                                  *
\*===========================================================================*/
static void keyboard(unsigned char key, int, int)
{
  static bool wireframe = false;
  switch (key)
  {
    case 'q':
    case 27:
      exit (0);
    case 'w':
      wireframe = !wireframe;
      glPolygonMode(GL_FRONT_AND_BACK, wireframe ? GL_LINE : GL_FILL);
      break;
  }
  glutPostRedisplay();
}

/*===========================================================================*\
 * special                                                                   *
\*===========================================================================*/
static void special(int key, int, int)
{
  switch (key)
  {
    case GLUT_KEY_LEFT:
      angle_y-=5;
      break;
    case GLUT_KEY_RIGHT:
      angle_y+=5;
      break;
    case GLUT_KEY_UP:
      pos_x++;
      if (pos_x > 520)
        pos_x = 520;
      break;
    case GLUT_KEY_DOWN:
      pos_x--;
      if (pos_x < 300)
        pos_x = 300;
      break;
    case GLUT_KEY_PAGE_UP:
      alt_y++;
      break;
    case GLUT_KEY_PAGE_DOWN:
      alt_y--;
      break;
  }
  glutPostRedisplay();
}

/*===========================================================================*\
 * load_noise_vol                                                            *
\*===========================================================================*/
static void load_noise_vol(const char *filename)
{
  GLubyte *array;
  std::cerr << "loading " << filename << std::endl;
  FILE *file = fopen(filename, "r");
  if (!file)
  {
    std::cerr << "can't read noise file ("<<filename<<") !!!" << std::endl;
    return;
  }
  int a[5];
  fread(a, sizeof(int), 5, file);

  array = new GLubyte[a[4]];
  fread(array, 1, a[4], file);
  fclose(file);

  int i=0; GLubyte *p;
  for (p = array;a[0]*a[1]*a[2]!=0;p+=a[0]*a[1]*a[2]*a[3],a[0]/=2,a[1]/=2,a[2]/=2,i++)
    glTexImage3D(GL_TEXTURE_3D, i, GL_RGBA16F, a[0], a[1], a[2], 0, GL_RGBA, GL_HALF_FLOAT, p);
  delete[] array;
}

/*===========================================================================*\
 * compute_density_pass_init                                                 *
\*===========================================================================*/
static void compute_density_pass_init()
{
  glGenFramebuffers(1, &fb);
  glGenTextures(1, &density_tex);
  glBindFramebuffer(GL_FRAMEBUFFER, fb);
  glActiveTexture(GL_TEXTURE3);
  glBindTexture(GL_TEXTURE_3D, density_tex);
  glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
  glTexImage3D(GL_TEXTURE_3D, 0, GL_R32F, 48, 48, 48, 0, GL_RED, GL_FLOAT, nullptr);
  glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, density_tex, 0);
  shader_density->activate();

//  CHECK_FRAMEBUFFER_STATUS();

  // vbo/vao
  GLfloat coords[] = { -1,-1, 1,-1, 1,1, -1,1 };
  glGenVertexArrays(1, &density_vao);
  glBindVertexArray(density_vao);
  glGenBuffers(1, &density_vbo);
  glBindBuffer(GL_ARRAY_BUFFER, density_vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof coords, coords, GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
  glBindVertexArray(0);
}

/*===========================================================================*\
 * init                                                                      *
\*===========================================================================*/
static void init()
{
  int count = 34500; // ???
  Image  *image;
 // GLfloat lposition[] = { 1, 0.5, 5, 0 };

  compute_density_pass_init();
  glActiveTexture(GL_TEXTURE0);
  image = image_load_tga("textures/texture.tga");
  if (image)
  {
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glGenTextures(1, &surfacetex);
    glBindTexture(GL_TEXTURE_2D, surfacetex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image->width, image->height, 0, GL_RGB, GL_UNSIGNED_BYTE, image->data);
  }

  glActiveTexture(GL_TEXTURE4);
  load_noise_vol("textures/noise_half_16cubed_mips_01.vol");
  glActiveTexture(GL_TEXTURE5);
  load_noise_vol("textures/noise_half_16cubed_mips_01.vol");
  /*  glActiveTexture (GL_TEXTURE6);
      load_noise_vol ("textures/noise_half_16cubed_mips_02.vol");*/
  /*  glActiveTexture (GL_TEXTURE7);
      load_noise_vol ("textures/noise_half_16cubed_mips_03.vol");
      */

  marching_cube_gpu_setup();

  glClearColor(.6, .7, .8, 0);
  glEnable(GL_DEPTH_TEST);

  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);

  /* the feedback buffer for the list triangle pass */
  glGenVertexArrays(1, &list_triangle_vao);
  glBindVertexArray(list_triangle_vao);
  glGenBuffers(1, &lt_pos_vbo);
  glBindBuffer(GL_ARRAY_BUFFER, lt_pos_vbo);
  glBufferData(GL_ARRAY_BUFFER, count * 3 * sizeof(float), nullptr, GL_STREAM_COPY);
  glGenBuffers(1, &lt_edge_vbo);
  glBindBuffer(GL_ARRAY_BUFFER, lt_edge_vbo);
  glBufferData(GL_ARRAY_BUFFER, count * 3 * sizeof(float), nullptr, GL_STREAM_COPY);

  /* The feedback buffers for gen vertices pass */
  glGenVertexArrays(1, &generate_vertices_vao);
  glBindVertexArray(generate_vertices_vao);

  GLuint buffers[2 * NB_BLOCKS];
  glGenBuffers (2 * NB_BLOCKS, buffers);
  for (int i = 0; i < NB_BLOCKS; i++)
  {
    blocks[i].nb_vertices = 0;
    blocks[i].vertices_vbo = buffers[2 * i];
    blocks[i].normals_vbo  = buffers[2 * i + 1];
    glBindBuffer(GL_ARRAY_BUFFER, blocks[i].vertices_vbo);
    glBufferData(GL_ARRAY_BUFFER, count * 4 * sizeof(float), 0, GL_STATIC_COPY);
    glBindBuffer(GL_ARRAY_BUFFER, blocks[i].normals_vbo);
    glBufferData(GL_ARRAY_BUFFER, count * 3 * sizeof(float), 0, GL_STATIC_COPY);
  }
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glActiveTexture(GL_TEXTURE3);
  glBindTexture(GL_TEXTURE_3D, density_tex);

  glGenVertexArrays(1, &render_vao);
}

/*===========================================================================*\
 * idle                                                                      *
\*===========================================================================*/
static void idle()
{
  glutPostRedisplay();
}

int main(int argc, char *argv[])
{
  glutInitContextVersion(3, 3);
  glutInitContextProfile(GLUT_CORE_PROFILE);
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
  glutInitWindowSize(win_width, win_height);
  glutCreateWindow("Marching Cubes");

  glewExperimental=true;glewInit();glGetError();
  print_stats();

  create_shader();

  init();

  glutDisplayFunc(display);
  glutKeyboardFunc(keyboard);
  glutSpecialFunc(special);
  glutReshapeFunc(reshape);
  glutIdleFunc(idle);
  glutMainLoop();
}

