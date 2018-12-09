#version 330

//Volume data field texture
uniform sampler3D dataFieldTex;

in vec3 position;

out int bla;

void main(void)
{
  gl_Position = vec4(position, 1.0);
  vec2 size = vec2 (1 / 48.0, 0.0);
  vec3 pos = ((position.xyz - 16.0) / 24.0 + 1.0) / 2.0;
  vec4 f0123 = vec4(texture(dataFieldTex, pos + size.yyy).r,
                    texture(dataFieldTex, pos + size.xyy).r,
                    texture(dataFieldTex, pos + size.xxy).r,
                    texture(dataFieldTex, pos + size.yxy).r);
  vec4 f4567 = vec4(texture(dataFieldTex, pos + size.yyx).r,
                    texture(dataFieldTex, pos + size.xyx).r,
                    texture(dataFieldTex, pos + size.xxx).r,
                    texture(dataFieldTex, pos + size.yxx).r);
  ivec4 n0123 = ivec4(clamp(f0123 * -99999.0, 0.0, 1.0)); 
  ivec4 n4567 = ivec4(clamp(f4567 * -99999.0, 0.0, 1.0)); 

  bla = (n0123.x     ) | (n4567.x << 4) |
        (n0123.y << 1) | (n4567.y << 5) |
        (n0123.z << 2) | (n4567.z << 6) |
        (n0123.w << 3) | (n4567.w << 7);
}
