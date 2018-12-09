#version 330 

layout(triangles) in;
layout(triangle_strip,max_vertices = 16) out;

out vec4 position;
in float layer[];

void main (void)
{
  for (int i = 0; i < gl_in.length(); i++)
  {
    gl_Position = gl_in[i].gl_Position;
    position = gl_in[i].gl_Position;
    gl_Layer = int(layer[i]);
    position.z = (layer[i] - 24.0) / 24.0;
    EmitVertex();
  }
} 
