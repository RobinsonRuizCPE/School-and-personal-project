// shader.cpp

#include <iostream>
#include <string>

#define GLEW_STATIC 1
#include <GL/glew.h>
#include <GL/freeglut.h>

#include "gl-utils.h"

#include "shader.h"

/*---------------------------------------------------------------------------*\
 * Shader::compile                                                           *
\*---------------------------------------------------------------------------*/
void Shader::compile(unsigned shader, const std::string &source, const std::string &type)
{
  int         info_log_length = 0;
  GLint       status;
  const char *c_src = source.c_str();
  glShaderSource(shader, 1, &c_src, 0); PRINT_OPENGL_ERROR();
  glCompileShader(shader);              PRINT_OPENGL_ERROR();
  glGetShaderiv(shader, GL_COMPILE_STATUS, &status); PRINT_OPENGL_ERROR();
  glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &info_log_length); PRINT_OPENGL_ERROR();
  if (info_log_length > 1)
  {
    int     chars_written = 0;
    GLchar *info_log = new GLchar[info_log_length];
    glGetShaderInfoLog(shader, info_log_length, &chars_written, info_log); PRINT_OPENGL_ERROR();
    std::cerr << "Shader compile error (" << type << "): " << source << std::endl;
    std::cerr << "Shader compile error " << info_log << std::endl;
    delete[] info_log;
    return;
  }
  glAttachShader(program, shader); PRINT_OPENGL_ERROR();
}

/*=========================================================================*\
 * get_file_content                                                        *
\*=========================================================================*/
static std::string get_file_content(const std::string &filename)
{
  std::string ret = "";
  if (filename == "")
    return "";
  if(FILE *fp = fopen(filename.c_str(), "r"))
  {
    char buf[1024];
    while(size_t len = fread(buf, 1, sizeof(buf), fp))
      ret += std::string(buf, buf + len);
    fclose (fp);
  }
  return ret;
}

/***************************************************************************\
 * Shader::Shader                                                          *
\***************************************************************************/
Shader::Shader()
{
  program = glCreateProgram(); PRINT_OPENGL_ERROR();
}

/***************************************************************************\
 * Shader::attach_vertex                                                   *
\***************************************************************************/
void Shader::attach_vertex(const std::string &source)
{
  int     info_log_length = 0;
  int     chars_written   = 0;
  GLchar *info_log;
  GLint   status;
  const char *c_src = source.c_str();

  vertex_shader = glCreateShader(GL_VERTEX_SHADER); PRINT_OPENGL_ERROR();
  glShaderSource(vertex_shader, 1, &c_src, 0); PRINT_OPENGL_ERROR();
  glCompileShader(vertex_shader); PRINT_OPENGL_ERROR();
  glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &status); PRINT_OPENGL_ERROR();
  glGetShaderiv(vertex_shader, GL_INFO_LOG_LENGTH, &info_log_length); PRINT_OPENGL_ERROR();

  if (info_log_length > 1)
  {
    info_log = new GLchar[info_log_length]; // FIXME! use vector
    glGetShaderInfoLog(vertex_shader, info_log_length, &chars_written, info_log); PRINT_OPENGL_ERROR();
    std::cerr << "Problem compiling:\n" << source << "\n" << info_log;
    delete[] info_log;
  }
  glAttachShader(program, vertex_shader); PRINT_OPENGL_ERROR();
}

/***************************************************************************\
 * Shader::attach_vertex_from_file                                         *
\***************************************************************************/
void Shader::attach_vertex_from_file(const std::string &filename)
{
  attach_vertex(get_file_content(filename));
}

/***************************************************************************\
 * Shader::attach_fragment                                                 *
\***************************************************************************/
void Shader::attach_fragment(const std::string &source)
{
  int     info_log_length = 0;
  int     chars_written   = 0;
  GLchar *info_log;
  GLint   status;
  const char *c_src = source.c_str();

  fragment_shader = glCreateShader(GL_FRAGMENT_SHADER); PRINT_OPENGL_ERROR();
  glShaderSource(fragment_shader, 1, &c_src, 0); PRINT_OPENGL_ERROR();
  glCompileShader(fragment_shader); PRINT_OPENGL_ERROR();
  glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &status); PRINT_OPENGL_ERROR();
  glGetShaderiv(fragment_shader, GL_INFO_LOG_LENGTH, &info_log_length); PRINT_OPENGL_ERROR();

  if (info_log_length > 1)
  {
    info_log = new GLchar[info_log_length]; // FIXME! use vector
    glGetShaderInfoLog(fragment_shader, info_log_length, &chars_written, info_log); PRINT_OPENGL_ERROR();
    std::cerr << "Problem compiling:\n" << source << "\n" << info_log;
    delete[] info_log;
  }
  glAttachShader(program, fragment_shader); PRINT_OPENGL_ERROR();
}

/***************************************************************************\
 * Shader::attach_fragment_from_file                                       *
\***************************************************************************/
void Shader::attach_fragment_from_file(const std::string &filename)
{
  attach_fragment(get_file_content(filename));
}

/*****************************************************************************\
 * Shader::attach_geometry                                                   *
\*****************************************************************************/
void Shader::attach_geometry(const std::string &source)
{
  geometry_shader = glCreateShader(GL_GEOMETRY_SHADER); PRINT_OPENGL_ERROR();
  compile(geometry_shader, source, "Geometry shader");
}

/*****************************************************************************\
 * Shader::attach_geometry_from_file                                         *
\*****************************************************************************/
void Shader::attach_geometry_from_file(const std::string &filename)
{
  std::string source = get_file_content(filename);
  geometry_shader = glCreateShader(GL_GEOMETRY_SHADER); PRINT_OPENGL_ERROR();
  compile(geometry_shader, source, "Geometry shader " + filename);
}

/*****************************************************************************\
 * Shader::set_tf_targets                                                    *
\*****************************************************************************/
void Shader::set_tf_targets(const std::vector<std::string> &targets)
{
  const GLchar * *locations = new const GLchar *[targets.size()];

  for (unsigned int i = 0; i < targets.size(); i++)
    locations[i] = targets[i].c_str();
  glTransformFeedbackVaryings(program, targets.size(), locations, GL_SEPARATE_ATTRIBS); PRINT_OPENGL_ERROR();

  delete[] locations;
}

/***************************************************************************\
 * Shader::set_attributes                                                  *
\***************************************************************************/
void Shader::set_attributes(const std::vector<std::string> &attributes)
{
  for (unsigned i = 0; i < attributes.size(); i++)
  {
    glBindAttribLocation(program, i, attributes[i].c_str()); PRINT_OPENGL_ERROR();
  }
}

/***************************************************************************\
 * Shader::link                                                            *
\***************************************************************************/
void Shader::link()
{
  int     info_log_length = 0;
  int     chars_written  = 0;
  GLchar *info_log;

  glLinkProgram(program); PRINT_OPENGL_ERROR();
  glGetProgramiv(program, GL_INFO_LOG_LENGTH, &info_log_length); PRINT_OPENGL_ERROR();

  if (info_log_length > 1)
  {
    info_log = new GLchar[info_log_length];
    glGetProgramInfoLog (program, info_log_length, &chars_written, info_log); PRINT_OPENGL_ERROR();
    std::cerr << "Problem linking: " << info_log << std::endl;
    delete[] info_log;
  }
  glValidateProgram(program); PRINT_OPENGL_ERROR();
  glGetProgramiv(program, GL_VALIDATE_STATUS, &info_log_length); PRINT_OPENGL_ERROR();
  glGetProgramiv(program, GL_INFO_LOG_LENGTH, &info_log_length); PRINT_OPENGL_ERROR();

  if (info_log_length > 1)
  {
    info_log = new GLchar[info_log_length];
    glGetProgramInfoLog (program, info_log_length, &chars_written, info_log); PRINT_OPENGL_ERROR();
    std::cerr << "Problem validating: " << info_log << std::endl;
    delete[] info_log;
  }
}

/***************************************************************************\
 * Shader::activate                                                        *
\***************************************************************************/
void Shader::activate()
{
  glUseProgram(program); PRINT_OPENGL_ERROR();
}

/***************************************************************************\
 * Shader::set                                                             *
\***************************************************************************/
void Shader::set(const std::string &uniform, int value)
{
  GLint uniform_location = glGetUniformLocation(program, uniform.c_str()); PRINT_OPENGL_ERROR();
  if (uniform_location == -1)
  {
    std::cerr << "Can't find this (int) uniform: " << uniform << std::endl;
    return;
  }
  glUniform1i(uniform_location, value); PRINT_OPENGL_ERROR();
}

/***************************************************************************\
 * Shader::set                                                             *
\***************************************************************************/
void Shader::set(const std::string &uniform, float value)
{
  GLint uniform_location = glGetUniformLocation(program, uniform.c_str()); PRINT_OPENGL_ERROR();
  if (uniform_location == -1)
  {
    std::cerr << "Can't find this (float) uniform: " << uniform << std::endl;
    return;
  }
  glUniform1f(uniform_location, value); PRINT_OPENGL_ERROR();
}

/***************************************************************************\
 * Shader::set                                                             *
\***************************************************************************/
void Shader::set(const std::string &uniform, const vec2 &vec)
{
  GLint uniform_location = glGetUniformLocation(program, uniform.c_str()); PRINT_OPENGL_ERROR();
  if (uniform_location == -1)
  {
    std::cerr << "Can't find this (vec2) uniform: " << uniform << std::endl;
    return;
  }
  glUniform2fv(uniform_location, 1, &vec.x); PRINT_OPENGL_ERROR();
}

/***************************************************************************\
 * Shader::set                                                             *
\***************************************************************************/
void Shader::set(const std::string &uniform, const vec3 &vec)
{
  GLint uniform_location = glGetUniformLocation(program, uniform.c_str()); PRINT_OPENGL_ERROR();
  if (uniform_location == -1)
  {
    std::cerr << "Can't find this (vec3) uniform: " << uniform << std::endl;
    return;
  }
  glUniform3fv(uniform_location, 1, &vec.x); PRINT_OPENGL_ERROR();
}

/***************************************************************************\
 * Shader::set                                                             *
\***************************************************************************/
void Shader::set(const std::string &uniform, const vec4 &vec)
{
  GLint uniform_location = glGetUniformLocation(program, uniform.c_str()); PRINT_OPENGL_ERROR();
  if (uniform_location == -1)
  {
    std::cerr << "Can't find this (vec4) uniform: " << uniform << std::endl;
    return;
  }
  glUniform4fv(uniform_location, 1, &vec.x); PRINT_OPENGL_ERROR();
}

/***************************************************************************\
 * Shader::set                                                             *
\***************************************************************************/
void Shader::set(const std::string &uniform, float x, float y, float z)
{
  GLint uniform_location = glGetUniformLocation(program, uniform.c_str()); PRINT_OPENGL_ERROR();
  if (uniform_location == -1)
  {
    std::cerr << "Can't find this (vec3) uniform: " << uniform << std::endl;
    return;
  }
  glUniform3f(uniform_location, x, y, z); PRINT_OPENGL_ERROR();
}

/***************************************************************************\
 * Shader::set                                                             *
\***************************************************************************/
void Shader::set(const std::string &uniform, const matrix &matrix)
{
  GLint uniform_location = glGetUniformLocation(program, uniform.c_str()); PRINT_OPENGL_ERROR();
  if (uniform_location == -1)
  {
    std::cerr << "Can't find this (mat4) uniform: " << uniform << std::endl;
    return;
  }
  glUniformMatrix4fv(uniform_location, 1, GL_FALSE, matrix.m); PRINT_OPENGL_ERROR();
}

/***************************************************************************\
 * Shader::set                                                             *
\***************************************************************************/
void Shader::set_matrix16f(const std::string &uniform, const float *matrix)
{
  GLint uniform_location = glGetUniformLocation(program, uniform.c_str()); PRINT_OPENGL_ERROR();
  if (uniform_location == -1)
  {
    std::cerr << "Can't find this (mat4) uniform: " << uniform << std::endl;
    return;
  }
  glUniformMatrix4fv(uniform_location, 1, GL_FALSE, matrix); PRINT_OPENGL_ERROR();
}

/***************************************************************************\
 * Shader::set                                                             *
\***************************************************************************/
void Shader::set(const std::string &uniform, unsigned count, const vec3 *values)
{
  GLint uniform_location = glGetUniformLocation(program, uniform.c_str()); PRINT_OPENGL_ERROR();
  if (uniform_location == -1)
  {
    std::cerr << "Can't find this (vec3v) uniform: " << uniform << std::endl;
    return;
  }
  glUniform3fv(uniform_location, count, &values[0].x); PRINT_OPENGL_ERROR();
}

/***************************************************************************\
 * Shader::set                                                             *
\***************************************************************************/
void Shader::set(const std::string &uniform, unsigned count, const vec4 *values)
{
  GLint uniform_location = glGetUniformLocation(program, uniform.c_str()); PRINT_OPENGL_ERROR();
  if (uniform_location == -1)
  {
    std::cerr << "Can't find this (vec4v) uniform: " << uniform << std::endl;
    return;
  }
  glUniform4fv(uniform_location, count, &values[0].x); PRINT_OPENGL_ERROR();
}

/***************************************************************************\
 * Shader::get_attrib_location                                             *
\***************************************************************************/
int Shader::get_attrib_location(const std::string &attrib)
{
  return glGetAttribLocation(program, attrib.c_str()); PRINT_OPENGL_ERROR();
}

