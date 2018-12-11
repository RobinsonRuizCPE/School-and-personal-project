// vec4.h

#ifndef VEC4_H
#define VEC4_H

struct vec4
{
  float x, y, z, w;

  vec4 (float x_ = 0, float y_ = 0, float z_ = 0, float w_ = 1) : x(x_), y(y_), z(z_), w(w_) { }
};

#endif
