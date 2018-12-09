// matrix.h

#ifndef MATRIX_H
#define MATRIX_H

#include "vec3.h"

struct matrix
{
  float m[16];
  matrix operator*(const matrix &mat) const;
  matrix();
  void set_perspective(float fovy, float aspect, float near, float far);
  void set_look_at(const vec3 &eye, const vec3 &center, const vec3 &up);
  void rotate(float angle, const vec3 &v);
  void translate(const vec3 &v);
};

#endif
