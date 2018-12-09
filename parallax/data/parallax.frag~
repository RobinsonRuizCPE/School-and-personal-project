#version 330 core

// Variables d'entrée
in vec3 vf_frag_pos;
in vec2 vf_tex_coords;
in vec3 vf_tangent_light_pos;
in vec3 vf_tangent_view_pos;
in vec3 vf_tangent_frag_pos;

// Variable de sortie
out vec4 color;

// Textures
uniform sampler2D diffuse_tex;
uniform sampler2D normal_tex;
//uniform sampler2D height_tex;

// Paramètres
// uniform ... 

void main()
{
  vec3 view_dir = normalize(vf_tangent_view_pos - vf_tangent_frag_pos);
  vec2 tex_coords = vf_tex_coords;

  // Pour le parallax mapping, il faudra adapter les coordonnées de texture
  
  // On laisse tomber les pixels qui sont en dehors de la texture (pour le parallax mapping)
  if (tex_coords.x > 1.0 || tex_coords.y > 1.0 || tex_coords.x < 0.0 || tex_coords.y < 0.0)
    discard;

  // Récupération de la normale depuis la normal map
  vec3 normal = texture(normal_tex, tex_coords).xyz;
  normal = normalize(normal * 2.0 - 1.0); // passe de 0->1 à -1->1

  // Récupération de la couleur de base
  vec3 base_color = texture(diffuse_tex, tex_coords).rgb;
  // Ambient
  vec3 ambient = 0.1 * base_color;
  // Diffuse
  vec3 light_dir = normalize(vf_tangent_light_pos - vf_tangent_frag_pos);
  float diff = max(dot(light_dir, normal), 0.0);
  vec3 diffuse = diff * base_color;
  // Specular    
  vec3 reflect_dir = reflect(-light_dir, normal);
  vec3 halfway_dir = normalize(light_dir + view_dir);  
  float spec = pow(max(dot(normal, halfway_dir), 0.0), 32.0);

  vec3 specular = vec3(0.2) * spec;
  color = vec4(ambient + diffuse + specular, 1.0);
}
