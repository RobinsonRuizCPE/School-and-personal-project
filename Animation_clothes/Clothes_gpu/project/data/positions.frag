#version 120

uniform sampler2D positions;
uniform sampler2D speeds;

#define DT 0.005

const vec3 sphere1 = vec3 (-0.7, 0.0, -0.8);
const float sphere1_radius = 0.7;
const vec3 sphere2 = vec3 (-0.4, 0.0, 0.5);
const float sphere2_radius = 0.6;
const vec2 cylinder = vec2 (1.0, 0.0);
const float cylinder_radius = 0.4;

void main()
{
  vec3 speed    = texture2D (speeds, gl_TexCoord[0].st).xyz;
  vec3 position = texture2D (positions, gl_TexCoord[0].st).xyz;

  position = position + DT*speed;

  // Check collision with sphere 1.
  if (distance (position, sphere1) < sphere1_radius)
    position = sphere1 + normalize (position - sphere1) * sphere1_radius;

  // Check collision with sphere 2.
  if (distance (position, sphere2) < sphere2_radius)
    position = sphere2 + normalize (position - sphere2) * sphere2_radius;

  // Check collision with the cylinder.
  if (distance (position.xy, cylinder) < cylinder_radius)
    position.xy = cylinder + normalize (position.xy - cylinder) * cylinder_radius;

  gl_FragColor = vec4 (position, 0.0);
}
