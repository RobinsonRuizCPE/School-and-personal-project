// shader.h

#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <vector>

#include "vec2.h"
#include "vec3.h"
#include "vec4.h"
#include "matrix.h"

class Shader
{
  unsigned program;
  unsigned vertex_shader;
  unsigned fragment_shader;
  unsigned geometry_shader;
  void compile(unsigned shader, const std::string &source, const std::string &type);
public:
  Shader();
  void attach_vertex(const std::string &source);
  void attach_vertex_from_file(const std::string &filename);
  void attach_fragment(const std::string &source);
  void attach_fragment_from_file(const std::string &filename);
  void attach_geometry(const std::string &source);
  void attach_geometry_from_file(const std::string &filename);
  void set_tf_targets(const std::vector<std::string> &targets);

  void set_attributes(const std::vector<std::string> &attributes);
  void link();
  void activate();
  void set(const std::string &uniform, int value);
  void set(const std::string &uniform, float value);
  void set(const std::string &uniform, float x, float y, float z);
  void set(const std::string &uniform, const vec2 &vec);
  void set(const std::string &uniform, const vec3 &vec);
  void set(const std::string &uniform, const vec4 &vec);
  void set(const std::string &uniform, const matrix &matrix);
  void set(const std::string &uniform, unsigned count, const vec3 *values);
  void set(const std::string &uniform, unsigned count, const vec4 *values);
  void set_matrix16f(const std::string &uniform, const float *matrix);

  int get_attrib_location(const std::string &attrib);
};

#endif
