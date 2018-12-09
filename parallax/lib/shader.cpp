// shader.cpp

#include <iostream>
#include <string>

#define GLEW_STATIC 1
#include <GL/glew.h>
#include <GL/freeglut.h>

#include "shader.h"

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
  program = glCreateProgram();
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

  vertex_shader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertex_shader, 1, &c_src, 0);
  glCompileShader(vertex_shader);
  glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &status);
  glGetShaderiv(vertex_shader, GL_INFO_LOG_LENGTH, &info_log_length);

  if (info_log_length > 1)
  {
    info_log = new GLchar[info_log_length]; // FIXME! use vector
    glGetShaderInfoLog(vertex_shader, info_log_length, &chars_written, info_log);
    std::cerr << "Problem compiling:\n" << source << "\n" << info_log;
    delete[] info_log;
  }
  glAttachShader(program, vertex_shader);
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

  fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragment_shader, 1, &c_src, 0);
  glCompileShader(fragment_shader);
  glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &status);
  glGetShaderiv(fragment_shader, GL_INFO_LOG_LENGTH, &info_log_length);

  if (info_log_length > 1)
  {
    info_log = new GLchar[info_log_length]; // FIXME! use vector
    glGetShaderInfoLog(fragment_shader, info_log_length, &chars_written, info_log);
    std::cerr << "Problem compiling:\n" << source << "\n" << info_log;
    delete[] info_log;
  }
  glAttachShader(program, fragment_shader);
}

/***************************************************************************\
 * Shader::attach_fragment_from_file                                       *
\***************************************************************************/
void Shader::attach_fragment_from_file(const std::string &filename)
{
  attach_fragment(get_file_content(filename));
}

/***************************************************************************\
 * Shader::set_attributes                                                  *
\***************************************************************************/
void Shader::set_attributes(const std::vector<std::string> &attributes)
{
  for (unsigned i = 0; i < attributes.size(); i++)
    glBindAttribLocation(program, i, attributes[i].c_str());
}

/***************************************************************************\
 * Shader::link                                                            *
\***************************************************************************/
void Shader::link()
{
  int     info_log_length = 0;
  int     chars_written  = 0;
  GLchar *info_log;

  glLinkProgram(program);
  glGetProgramiv(program, GL_INFO_LOG_LENGTH, &info_log_length);

  if (info_log_length > 1)
  {
    info_log = new GLchar[info_log_length];
    glGetProgramInfoLog (program, info_log_length, &chars_written, info_log);
    std::cerr << "Problem linking: " << info_log;
    delete[] info_log;
  }
  glValidateProgram(program);
  glGetProgramiv(program, GL_VALIDATE_STATUS, &info_log_length);
  glGetProgramiv(program, GL_INFO_LOG_LENGTH, &info_log_length);

  if (info_log_length > 1)
  {
    info_log = new GLchar[info_log_length];
    glGetProgramInfoLog (program, info_log_length, &chars_written, info_log);
    std::cerr << "Problem validating: " << info_log;
    delete[] info_log;
  }
}

/***************************************************************************\
 * Shader::activate                                                        *
\***************************************************************************/
void Shader::activate()
{
  glUseProgram(program);
}

/***************************************************************************\
 * Shader::set                                                             *
\***************************************************************************/
void Shader::set(const std::string &uniform, int value)
{
  GLint uniform_location = glGetUniformLocation(program, uniform.c_str());
  if (uniform_location == -1)
  {
    std::cerr << "Can't find this (int) uniform: " << uniform << std::endl;
    return;
  }
  glUniform1i(uniform_location, value);
}

/***************************************************************************\
 * Shader::set                                                             *
\***************************************************************************/
void Shader::set(const std::string &uniform, float value)
{
  GLint uniform_location = glGetUniformLocation(program, uniform.c_str());
  if (uniform_location == -1)
  {
    std::cerr << "Can't find this (float) uniform: " << uniform << std::endl;
    return;
  }
  glUniform1f(uniform_location, value);
}

/***************************************************************************\
 * Shader::set                                                             *
\***************************************************************************/
void Shader::set(const std::string &uniform, const vec2 &vec)
{
  GLint uniform_location = glGetUniformLocation(program, uniform.c_str());
  if (uniform_location == -1)
  {
    std::cerr << "Can't find this (vec2) uniform: " << uniform << std::endl;
    return;
  }
  glUniform2fv(uniform_location, 1, &vec.x);
}

/***************************************************************************\
 * Shader::set                                                             *
\***************************************************************************/
void Shader::set(const std::string &uniform, const vec3 &vec)
{
  GLint uniform_location = glGetUniformLocation(program, uniform.c_str());
  if (uniform_location == -1)
  {
    std::cerr << "Can't find this (vec3) uniform: " << uniform << std::endl;
    return;
  }
  glUniform3fv(uniform_location, 1, &vec.x);
}

/***************************************************************************\
 * Shader::set                                                             *
\***************************************************************************/
void Shader::set(const std::string &uniform, const vec4 &vec)
{
  GLint uniform_location = glGetUniformLocation(program, uniform.c_str());
  if (uniform_location == -1)
  {
    std::cerr << "Can't find this (vec4) uniform: " << uniform << std::endl;
    return;
  }
  glUniform4fv(uniform_location, 1, &vec.x);
}

/***************************************************************************\
 * Shader::set                                                             *
\***************************************************************************/
void Shader::set(const std::string &uniform, float x, float y, float z)
{
  GLint uniform_location = glGetUniformLocation(program, uniform.c_str());
  if (uniform_location == -1)
  {
    std::cerr << "Can't find this (vec3) uniform: " << uniform << std::endl;
    return;
  }
  glUniform3f(uniform_location, x, y, z);
}

/***************************************************************************\
 * Shader::set                                                             *
\***************************************************************************/
void Shader::set(const std::string &uniform, const matrix &matrix)
{
  GLint uniform_location = glGetUniformLocation(program, uniform.c_str());
  if (uniform_location == -1)
  {
    std::cerr << "Can't find this (mat4) uniform: " << uniform << std::endl;
    return;
  }
  glUniformMatrix4fv(uniform_location, 1, GL_FALSE, matrix.m);
}

/***************************************************************************\
 * Shader::set                                                             *
\***************************************************************************/
void Shader::set_matrix16f(const std::string &uniform, const float *matrix)
{
  GLint uniform_location = glGetUniformLocation(program, uniform.c_str());
  if (uniform_location == -1)
  {
    std::cerr << "Can't find this (mat4) uniform: " << uniform << std::endl;
    return;
  }
  glUniformMatrix4fv(uniform_location, 1, GL_FALSE, matrix);
}

/***************************************************************************\
 * Shader::set                                                             *
\***************************************************************************/
void Shader::set(const std::string &uniform, unsigned count, const vec3 *values)
{
  GLint uniform_location = glGetUniformLocation(program, uniform.c_str());
  if (uniform_location == -1)
  {
    std::cerr << "Can't find this (vec3v) uniform: " << uniform << std::endl;
    return;
  }
  glUniform3fv(uniform_location, count, &values[0].x);
}

/***************************************************************************\
 * Shader::set                                                             *
\***************************************************************************/
void Shader::set(const std::string &uniform, unsigned count, const vec4 *values)
{
  GLint uniform_location = glGetUniformLocation(program, uniform.c_str());
  if (uniform_location == -1)
  {
    std::cerr << "Can't find this (vec4v) uniform: " << uniform << std::endl;
    return;
  }
  glUniform4fv(uniform_location, count, &values[0].x);
}

/***************************************************************************\
 * Shader::get_attrib_location                                             *
\***************************************************************************/
int Shader::get_attrib_location(const std::string &attrib)
{
  return glGetAttribLocation(program, attrib.c_str());
}

