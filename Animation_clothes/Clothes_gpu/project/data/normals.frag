#version 120

#define SIZE   128.0
#define DELTA  (1.0 / SIZE)

uniform sampler2D positions;

void main (void)
{
  vec3 v1 = texture2D (positions, gl_TexCoord[0].st + vec2 ( DELTA, 0.0)).xyz
          - texture2D (positions, gl_TexCoord[0].st + vec2 (-DELTA, 0.0)).xyz;
  vec3 v2 = texture2D (positions, gl_TexCoord[0].st + vec2 (0.0,  DELTA)).xyz
          - texture2D (positions, gl_TexCoord[0].st + vec2 (0.0, -DELTA)).xyz;

  gl_FragColor = vec4 (-normalize (cross (v1, v2)), 0.0);
}
