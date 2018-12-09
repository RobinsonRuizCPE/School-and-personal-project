#version 330

in vec4 position;

uniform sampler3D noiseVol0;
uniform sampler3D noiseVol1;
uniform sampler3D noiseVol2;
uniform sampler3D noiseVol3;


float smoothy(float t) { return t * t * (3 - 2 * t); }
vec2  smoothy(vec2  t) { return t * t * (3 - 2 * t); }
vec3  smoothy(vec3  t) { return t * t * (3 - 2 * t); }
vec4  smoothy(vec4  t) { return t * t * (3 - 2 * t); }

vec4 Noise_LQ_unsigned(vec3 uvw, sampler3D noiseTex)
{
  return texture(noiseTex, uvw);
}

vec4 Noise_LQ_signed(vec3 uvw, sampler3D noiseTex)
{
  return Noise_LQ_unsigned(uvw, noiseTex) * 2 - 1;
}

#define NOISE_LATTICE_SIZE 16

vec4 Noise_MQ_unsigned(vec3 uvw, sampler3D noiseTex)
{
  vec3 t = fract(uvw * NOISE_LATTICE_SIZE);
  vec3 t2 = (3 - 2 * t) * t * t;
  vec3 uvw2 = uvw + (t2 - t) /  NOISE_LATTICE_SIZE;
  return Noise_LQ_unsigned(uvw2, noiseTex);
}

vec4 Noise_MQ_signed(vec3 uvw, sampler3D noiseTex)
{
  vec3 t = fract(uvw * NOISE_LATTICE_SIZE);
  vec3 t2 = (3 - 2 * t) * t * t;
  vec3 uvw2 = uvw + (t2 - t) / NOISE_LATTICE_SIZE;
  return Noise_LQ_signed(uvw2, noiseTex);
}

//////////////////////////////////density functions ///////////////////////

float sdRoundCone( in vec3 p, in float r1, float r2, float h )
{
vec2 q = vec2( length(p.xz), p.y );

float b = (r1-r2)/h;
float a = sqrt(1.0-b*b);
float k = dot(q,vec2(-b,a));

if( k < 0.0 ) return length(q) - r1;
if( k > a*h ) return length(q-vec2(0.0,h)) - r2;

return dot(q, vec2(a,b) ) - r1;
}

float sdTorus( vec3 p, vec2 t )
{
  vec2 q = vec2(length(p.xz)-t.x,p.y);
  return length(q)-t.y;
}

float sdSphere( vec3 p, vec3 c, float s )
{
return length(p+c)-s;
}

float opSmoothUnion( float d1, float d2, float k )
{
float h = clamp( 0.5 + 0.5*(d2-d1)/k, 0.0, 1.0 );
return mix( d2, d1, h ) - k*h*(1.0-h);
}
/*
float opUnion( float d1, float d2 )
{
  return  min(d1,d2);
}
*/
vec3 opCheapBend(in vec3 p )
{
float c = cos(2.0*p.y);
float s = sin(2.0*p.y);
mat2  m = mat2(c,-s,s,c);
vec3  q = vec3(m*p.xy,p.z);
return q;
}


vec3 opRotation(vec3 p, vec3 u, float theta)
{


    //A CHECK
    u = normalize(u);
    float c = cos(theta);
    float s = sin(theta);
    mat4 R = mat4(u.x*u.x*(1-c)+c, u.x * u.y * (1-c) + u.y*s, u.x*u.z*(1-c) + u.y*s,1.0,
                  u.x*u.y*(1-c) + u.y*s, u.y*u.y*(1-c)+c, u.y*u.z*(1-c) + u.x*s,1.0,
                  u.x*u.z*(1-c) + u.y*s, u.y*u.z*(1-c) + u.x*s, u.z*u.z*(1-c)+c,1.0,
		1.0,1.0,1.0,1.0);

    vec4 projp = R*vec4(p,1.0);
    return vec3(projp.x,projp.y,projp.z);
}


float density(vec3 ws)
{
    // C'est cette fonction qui définit la "forme"

  //vec3 pillar = vec3(0.0,0.0,0.0);
  vec3 pillar2 = vec3(1.0,0.5,0.0);

  //float d = 0.1*sin(20*ws.x)*sin(20*ws.y)*sin(20*ws.z); //fonction deplacement

  //BOX
/*
  vec3 b= vec3(0.5,2.0,0.5);
  vec3 d = abs(ws) - b;
  return length(max(d,0.0)) + min(max(d.x,max(d.y,d.z)),0.0);

  //HELIX
  float c = cos(5.0*ws.z);
  float s = sin(5.0*ws.z);
  mat2  m = mat2(c,-s,s,c);
  vec3  q1 = vec3(m*ws.xy,ws.z);

  vec2 q = vec2(length(q1.xz)-pillar2.x,q1.y);
  //float q = 1.f - length(q1);
  return length(q)-pillar2.y;

  float f = 1.f - length(ws);
  //return f;
  //vec2 vec = vec2(cos(ws.x), sin(ws.x));
  //f += dot(vec, ws.yz);

  f += 1.f/length(ws.xy - pillar.xz) - 1.f;
  f += 1.f/length(ws.xy - pillar2.xz) - 1.f;
  f += cos(2.5*ws.z + 6);
*/
  vec3 p = opCheapBend(ws);
  vec4 f = Noise_MQ_signed(p,noiseVol1);
  vec2 q = vec2(length(f.xz)-pillar2.x,f.y);

  return length(q)-pillar2.y;
}


uniform vec3 offset;

out vec4 color;

void main(void)
{
  vec3 ws = position.xyz * 3.0 / 2.0 + offset;
  color.r = density(ws);
}
