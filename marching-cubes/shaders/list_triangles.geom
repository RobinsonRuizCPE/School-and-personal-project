#version 330

layout (points) in;
layout (points, max_vertices = 5) out;

//Volume data field texture
uniform sampler3D dataFieldTex;
//Triangles table textures
uniform isampler1D nb_tri_tex;
uniform isampler2D triTableTex; // FIXME: opt => edgetableTex.

in int bla[];
out vec3 outtripos;
out vec3 outtriedge;

//Get triangle table value
int triTableValue(int i, int j){
	return texelFetch(triTableTex, ivec2(j, i), 0).a;
}

void main(void)
{
  int cubeindex = bla[0];

  //Cube is entirely in/out of the surface
  if (cubeindex == 0 || cubeindex == 255) return;

  int i = 0;
  int j;
  int nb_tri = texelFetch(nb_tri_tex, cubeindex, 0).a;

  outtripos = gl_in[0].gl_Position.xyz;
  for (j = 0; j < min(nb_tri, 5); j++)
  {
    outtriedge = vec3(triTableValue(cubeindex, i + 0), triTableValue(cubeindex, i + 1), triTableValue(cubeindex, i + 2));
    EmitVertex();
    EndPrimitive();
    i = i + 3;
  }
}
