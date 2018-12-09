#version 330 core

// Variables d'entrée
in vec3 position;
in vec3 normal;
in vec2 tex_coords;
in vec3 tangent;
in vec3 bitangent;

// Variables de sortie
out vec3 vf_frag_pos;
out vec2 vf_tex_coords;
out vec3 vf_tangent_light_pos;
out vec3 vf_tangent_view_pos;
out vec3 vf_tangent_frag_pos;

// matrices
uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

// positions
uniform vec3 light_pos;
uniform vec3 view_pos;

void main()
{
  gl_Position = projection * view * model * vec4(position, 1.0);
  vf_frag_pos = vec3(model * vec4(position, 1.0));   
  vf_tex_coords = tex_coords;

  vec3 T = normalize(mat3(model) * tangent);
  vec3 B = normalize(mat3(model) * bitangent);
  vec3 N = normalize(mat3(model) * normal);
  mat3 TBN = transpose(mat3(T, B, N));

  vf_tangent_light_pos = TBN * light_pos;
  vf_tangent_view_pos  = TBN * view_pos;
  vf_tangent_frag_pos  = TBN * vf_frag_pos;
}
