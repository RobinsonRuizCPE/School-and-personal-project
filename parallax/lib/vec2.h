// vec2.h

#ifndef VEC2_H
#define VEC2_H

struct vec2
{
  float x, y;
  vec2(float x, float y) : x(x), y(y) {}
  vec2 operator+(const vec2 &v)const{return vec2(x+v.x,y+v.y);}
  vec2 operator-(const vec2 &v)const{return vec2(x-v.x,y-v.y);}
  vec2 operator*(const vec2 &v)const{return vec2(x*v.x,y*v.y);}
  vec2 operator/(const vec2 &v)const{return vec2(x/v.x,y/v.y);}
};

#endif
