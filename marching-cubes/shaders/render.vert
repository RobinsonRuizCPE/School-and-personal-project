#version 330

in vec4 position;
in vec3 normal;

uniform mat4 projection;
uniform mat4 modelview;

//Vertices position for fragment shader
out vec4  vf_position;
out vec4  vf_rawpos;
out vec4  vf_pos;
out vec3  vf_normal;
out float vf_occlusion;

void main(void)
{

	vf_position = vec4(position.xyz, 1.0);

  vf_rawpos = vec4(position.xyz, 1.0);
  vf_occlusion = position.w;
  vf_normal = normal;
  vf_pos = modelview * vf_position;
  gl_Position = projection * modelview * vf_position;
}
