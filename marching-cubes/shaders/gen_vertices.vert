#version 330

//Volume data field texture
uniform sampler3D dataFieldTex;

uniform sampler1D poisson;

uniform vec3 offset;

in vec3 position;
in vec3 normal;

out vec4 tridesc_wsCoordAmbo1;
out vec3 tridesc_wsNormal1;
out vec4 tridesc_wsCoordAmbo2;
out vec3 tridesc_wsNormal2;
out vec4 tridesc_wsCoordAmbo3;
out vec3 tridesc_wsNormal3;

//Get vertex i position within current marching cube
vec3 cubePos(int i){
  vec3 vert_decals[8];
  vert_decals[0] = vec3(0.0,        0.0,        0.0        );
  vert_decals[1] = vec3(1.0 / 24.0, 0.0,        0.0        );
  vert_decals[2] = vec3(1.0 / 24.0, 1.0 / 24.0, 0.0        );
  vert_decals[3] = vec3(0.0,        1.0 / 24.0, 0.0        );
  vert_decals[4] = vec3(0.0,        0.0,        1.0 / 24.0 );
  vert_decals[5] = vec3(1.0 / 24.0, 0.0,        1.0 / 24.0 );
  vert_decals[6] = vec3(1.0 / 24.0, 1.0 / 24.0, 1.0 / 24.0 );
  vert_decals[7] = vec3(0.0,        1.0 / 24.0, 1.0 / 24.0 );
  return (clamp(position, 0, 31) - 16.0) / 24.0 + vert_decals[i];
}

//Get vertex i value within current marching cube
float cubeVal(int i){
  return texture(dataFieldTex, (cubePos(i) + 1.0) / 2.0).r;
}

//Compute interpolated vertex along an edge
vec3 vertexInterp(vec3 v0, float l0, vec3 v1, float l1)
{
  // À modifier pour une vraie interpolation
  //return (v0 + v1 )/ 2;

   //if (abs(l0-l1) < 0.00001)
   //   return(v0);

   vec3 p;
   float mu = ( - l0) / (l1 - l0);
   p.x = v0.x + mu * (v1.x - v0.x);
   p.y = v0.y + mu * (v1.y - v0.y);
   p.z = v0.z + mu * (v1.z - v0.z);


  return p;
}

float text_density(vec3 pos)
{
  return texture(dataFieldTex, (pos + 1.0) / 2.0).r;
}

vec3 calc_normal(vec3 pos)
{
  // À modifier pour avoir une vraie normale
  float d = 1.0/256.0;
  vec3 grad;
  grad.x = text_density(pos + vec3( d, 0, 0)) -
           text_density(pos + vec3(-d, 0, 0));
  grad.y = text_density(pos + vec3( 0, d, 0)) -
           text_density(pos + vec3( 0,-d, 0));
  grad.z = text_density(pos + vec3( 0, 0, d)) -
           text_density(pos + vec3( 0, 0,-d));

  return -normalize(grad);
}

float occlusion(vec3 pos)
{
  float v = 0.0;
  float d;

  for (int ray = 0; ray < 32; ray++)
  {
    vec3 dir = texture(poisson, ray / 32.0).xyz;
    float this_ray_visibility = 1;
    vec3 pos_now = pos ;
    vec3 add_dir = dir / 24.0;
    for (int s = 1; s < 8; s++)
    {
      pos_now += add_dir;
      d = texture(dataFieldTex, pos_now).r;
      this_ray_visibility *= clamp(-d * 9999, 0, 1);
    }
    v += this_ray_visibility;
  }

  return 1.0 - v / 32.0;
}

void main(void)
{
  vec3 position;

  float cubeVal0 = cubeVal(0);
  float cubeVal1 = cubeVal(1);
  float cubeVal2 = cubeVal(2);
  float cubeVal3 = cubeVal(3);
  float cubeVal4 = cubeVal(4);
  float cubeVal5 = cubeVal(5);
  float cubeVal6 = cubeVal(6);
  float cubeVal7 = cubeVal(7);

  vec3 vertlist[12];
  int pos1 = int(clamp(normal.x, 0, 11));
  int pos2 = int(clamp(normal.y, 0, 11));
  int pos3 = int(clamp(normal.z, 0, 11));

  //Find the vertices where the surface intersects the cube
  vertlist[0]  = vertexInterp(cubePos(0), cubeVal0, cubePos(1), cubeVal1);
  vertlist[1]  = vertexInterp(cubePos(1), cubeVal1, cubePos(2), cubeVal2);
  vertlist[2]  = vertexInterp(cubePos(2), cubeVal2, cubePos(3), cubeVal3);
  vertlist[3]  = vertexInterp(cubePos(3), cubeVal3, cubePos(0), cubeVal0);
  vertlist[4]  = vertexInterp(cubePos(4), cubeVal4, cubePos(5), cubeVal5);
  vertlist[5]  = vertexInterp(cubePos(5), cubeVal5, cubePos(6), cubeVal6);
  vertlist[6]  = vertexInterp(cubePos(6), cubeVal6, cubePos(7), cubeVal7);
  vertlist[7]  = vertexInterp(cubePos(7), cubeVal7, cubePos(4), cubeVal4);
  vertlist[8]  = vertexInterp(cubePos(0), cubeVal0, cubePos(4), cubeVal4);
  vertlist[9]  = vertexInterp(cubePos(1), cubeVal1, cubePos(5), cubeVal5);
  vertlist[10] = vertexInterp(cubePos(2), cubeVal2, cubePos(6), cubeVal6);
  vertlist[11] = vertexInterp(cubePos(3), cubeVal3, cubePos(7), cubeVal7);

  //Generate first vertex of triangle
  position= vertlist[pos1];
  tridesc_wsCoordAmbo1.xyz = position * 3.0 / 2.0 + offset;
  tridesc_wsCoordAmbo1.w = occlusion((position + 1.0) / 2.0);
  tridesc_wsNormal1 = calc_normal(position);
  //Generate second vertex of triangle
  position= vertlist[pos2];
  tridesc_wsCoordAmbo2.xyz = position * 3.0 / 2.0 + offset;
  tridesc_wsCoordAmbo2.w = occlusion((position + 1.0) / 2.0);
  tridesc_wsNormal2 = calc_normal(position);
  //Generate last vertex of triangle
  position= vertlist[pos3];
  tridesc_wsCoordAmbo3.xyz = position * 3.0 / 2.0 + offset;
  tridesc_wsCoordAmbo3.w = occlusion((position + 1.0) / 2.0);
  tridesc_wsNormal3 = calc_normal(position);

  gl_Position = vec4(position, 1.0);
}
