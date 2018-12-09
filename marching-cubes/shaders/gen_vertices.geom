#version 330

layout (points) in;
layout (triangle_strip, max_vertices = 3) out;

struct v2g
{
  vec4 CoordAmbo[3];
  vec3 CoordNormal[3];
};

uniform vec3 offset;

in vec4 tridesc_wsCoordAmbo1[];
in vec3 tridesc_wsNormal1[];
in vec4 tridesc_wsCoordAmbo2[];
in vec3 tridesc_wsNormal2[];
in vec4 tridesc_wsCoordAmbo3[];
in vec3 tridesc_wsNormal3[];

in vec2 ws[];

out vec4 outvert;
out vec3 outnorm;

void main(void)
{
  // Just a passthought
  outvert = tridesc_wsCoordAmbo1[0];
  outnorm = tridesc_wsNormal1[0];
  gl_Position = outvert;
  EmitVertex();
  outvert = tridesc_wsCoordAmbo2[0];
  outnorm = tridesc_wsNormal2[0];
  gl_Position = outvert;
  EmitVertex();
  outvert = tridesc_wsCoordAmbo3[0];
  outnorm = tridesc_wsNormal3[0];
  gl_Position = outvert;
  EmitVertex();
  EndPrimitive();
}
