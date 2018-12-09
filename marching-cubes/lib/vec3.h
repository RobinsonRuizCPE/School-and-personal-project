// vec3.h

#ifndef VEC3_H
#define VEC3_H

#include <cmath>

struct vec3
{
  float x, y, z;
  vec3(float x, float y, float z) : x(x), y(y), z(z) {}
  vec3(float x = 0.0f) : x(x), y(x), z(x) {}
  vec3 operator+(const vec3&v)const{return vec3(x+v.x, y+v.y, z+v.z);}
  vec3 operator-(const vec3&v)const{return vec3(x-v.x, y-v.y, z-v.z);}
  vec3 operator*(const vec3&v)const{return vec3(x*v.x, y*v.y, z*v.z);}
  vec3 operator/(const vec3&v)const{return vec3(x/v.x, y/v.y, z/v.z);}

  vec3& operator+=(const vec3&v){x+=v.x;y+=v.y;z+=v.z;return*this;}
  vec3& operator-=(const vec3&v){x-=v.x;y-=v.y;z-=v.z;return*this;}
  vec3& operator*=(const vec3&v){x*=v.x;y*=v.y;z*=v.z;return*this;}
  vec3& operator/=(const vec3&v){x/=v.x;y/=v.y;z/=v.z;return*this;}

  vec3 operator-()const{return vec3(-x,-y,-z);}
  vec3 operator*(float f)const{return vec3(x*f,y*f,z*f);}
  vec3 operator/(float f)const{return vec3(x/f,y/f,z/f);}

  vec3& operator*=(float f){x*=f;y*=f;z*=f;return*this;}
  vec3& operator/=(float f){x/=f;y/=f;z/=f;return*this;}

  float dot(const vec3&v)const{return x*v.x+y*v.y+z*v.z;}
  float length()const{if (dot(*this)<0.001)return 1;return std::sqrt(dot(*this));}
  vec3& normalize(){float l=length();x/=l;y/=l;z/=l;return*this;}

  vec3 cross(const vec3&v)const{return vec3(y*v.z-z*v.y,z*v.x-x*v.z,x*v.y-y*v.x);}
  vec3 operator^(const vec3&v)const{return vec3(y*v.z-z*v.y,z*v.x-x*v.z,x*v.y-y*v.x);}
};

#endif
