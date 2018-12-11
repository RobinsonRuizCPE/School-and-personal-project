#version 120

uniform sampler2D positions;
uniform sampler2D speeds;
uniform sampler2D normals;

const float SIZE = 128.0;
#define SQRT2     (1.4142136)

const float DELTA  = (1.0 / SIZE);
#define GRID      (4.0 / SIZE)     //cloth has size 4x4 correspond to L0

#define DT 0.005
#define damping 50

const float GRAVITY = 9.81;
const float K_st = 4000.0;
const float K_sh = 3000.0;
const float K_be = 3000.0;

void main (void)
{

  float u = gl_TexCoord[0].s;
  float v = gl_TexCoord[0].t;
  vec2 uv = gl_TexCoord[0].st;

  vec3 speed  = texture2D (speeds   ,uv).xyz;
  vec3 pos    = texture2D (positions,uv).xyz;
  vec3 normal = texture2D (normals  ,uv).xyz;



  speed = (1-damping*DT)*speed;
  speed.y -= DT*GRAVITY;


  //verifie que les positions dans dans les limites attendues
  bool ok[4];
  ok[0] = u<(1-DELTA);  ok[1] = u>DELTA; ok[2] = v<(1-DELTA);  ok[3] = v>DELTA;
  for(int k=0;k<3;++k)
  {
    if( ok[k] )
    {
      //calculer ressorts et ajouter les forces à la vitesse
      // ex: vecteur_ressort_1 = texture2D(positions,uv+(1,0)*DELTA)-pos; ...
      // speed += DT * K * (longueur_repos-length(vecteur_ressort_1)) * vecteur_unitaire ...
      vec3 v_ressort_1 = texture2D(positions,uv+(1,0)*DELTA).xyz-pos;
      //speed += DT * K * (GRID-length(v_ressort_1)) * vecteur_unitaire;
      //1.0;
    }

  }


  gl_FragColor = vec4 (speed, 0.0);
}
