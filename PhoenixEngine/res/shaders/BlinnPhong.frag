/*******************************************************************************
Shader: PhongLighting Shader [Fragment]
Author: Ryan Buehler
*******************************************************************************/
#version 460 core
#define LIGHTCOUNT 16

// TODO: Looking for a better way than macro defines
#define POINT 0
#define DIRECTION 1
#define SPOT 2

uniform vec3 cam_position;

uniform vec3 global_amb;
uniform vec3 global_fog;
uniform float global_fog_near;
uniform float global_fog_far;
uniform float global_att1;
uniform float global_att2;
uniform float global_att3;

uniform vec3 mat_emit;
uniform float mat_amb;
uniform float mat_dif;
uniform float mat_spc;
uniform float mat_spc_exp;

struct Light
{
  vec4 Position;
  vec4 Ambience;
  vec4 Diffuse;
  vec4 Specular;
  vec4 Direction;
  int Type;
  float InnerFalloff;
  float OuterFalloff;
  bool IsActive;
};

uniform LightArray
{
  Light lights[LIGHTCOUNT];
};

in vec4 world_position;
in vec4 world_normal;
in vec2 uv;

out vec4 frag_color;

vec3 calcDirectionLight(int i, vec4 view_vector);
vec3 calcPointLight(int i, vec4 view_vector);
vec3 calcSpotLight(int i, vec4 view_vector);

void main(void)
{
  vec3 local = global_amb + mat_emit;

  // Calculate the view vector
  vec4 view_vector = vec4(cam_position, 1.f) - world_position;
  float view_vector_len = length(view_vector);
  vec4 view_vector_norm = normalize(view_vector);

  for(int i = 0; i < LIGHTCOUNT; ++i)
  {
    if(lights[i].IsActive)
    {
      switch(lights[i].Type)
      {
        // Point light
        case POINT:
          local += calcPointLight(i, view_vector_norm);
          break;
        // Directional light
        case DIRECTION:
          local += calcDirectionLight(i, view_vector_norm);
          break;
        // Spot light
        case SPOT:
          local += calcSpotLight(i, view_vector_norm);
          break;
        default:
          break;
      }
    }
  }

  // Fog
  float fog_value = (global_fog_far - view_vector_len) / (global_fog_far - global_fog_near);

  frag_color = vec4(fog_value * local + (1.f - fog_value) * global_fog, 1.f);
}

vec3 calcDirectionLight(int i, vec4 view_vector)
{
  // Calculate the light vector
  vec4 light_vector = normalize(-lights[i].Direction);
  
  // Calculate the half vector
  vec4 half_vector = normalize(light_vector + view_vector);

  vec3 ambient_value = lights[i].Ambience.xyz * mat_amb;
  vec3 diffuse_value = lights[i].Diffuse.xyz * mat_dif * max(dot(world_normal, light_vector), 0.f);
  vec3 specular_value = lights[i].Specular.xyz * mat_spc * pow(max(dot(world_normal, half_vector), 0.f), mat_spc_exp);

  return ambient_value + diffuse_value + specular_value;
}

vec3 calcPointLight(int i, vec4 view_vector)
{
  // Calculate the light vector
  vec4 light_vector = lights[i].Position - world_position;
  float light_vector_len = length(light_vector);
  vec4 light_vector_norm = normalize(light_vector);

  // Calculate the half vector
  vec4 half_vector = normalize(light_vector + view_vector);

  vec3 ambient_value = lights[i].Ambience.xyz * mat_amb;
  vec3 diffuse_value = lights[i].Diffuse.xyz * mat_dif * max(dot(world_normal, light_vector_norm), 0.f);
  vec3 specular_value = lights[i].Specular.xyz * mat_spc * pow(max(dot(world_normal, half_vector), 0.f), mat_spc_exp);

  float attenuation = global_att1 + global_att2 * light_vector_len + global_att3 * light_vector_len * light_vector_len;
  attenuation = min(1.f / attenuation, 1.f);
  return attenuation * (ambient_value + diffuse_value + specular_value);
}

vec3 calcSpotLight(int i, vec4 view_vector)
{
  // Calculate the light vector
  vec4 light_vector = lights[i].Position - world_position;
  float light_vector_len = length(light_vector);
  vec4 light_vector_norm = normalize(light_vector);

  // Calculate the difference between our light vector and the direction of the light
  float theta = dot(light_vector_norm, normalize(-lights[i].Direction));

  // Inner radius has full light
  float inner = cos(radians(lights[i].InnerFalloff));
  // Outer radius fades to nothing
  float outer = cos(radians(lights[i].OuterFalloff));

  if(theta > outer)
  {
    // Calculate the half vector
    vec4 half_vector = normalize(light_vector + view_vector);

    float falloff = 1.f - clamp((theta - inner) / (outer - inner), 0.f, 1.f);

    //return vec3(falloff, falloff, falloff);
    vec3 ambient_value = lights[i].Ambience.xyz * mat_amb;
    vec3 diffuse_value = falloff * lights[i].Diffuse.xyz * mat_dif * max(dot(world_normal, light_vector_norm), 0.f);
    vec3 specular_value = falloff * lights[i].Specular.xyz * mat_spc * pow(max(dot(world_normal, half_vector), 0.f), mat_spc_exp);

    float attenuation = global_att1 + global_att2 * light_vector_len + global_att3 * light_vector_len * light_vector_len;
    attenuation = min(1.f / attenuation, 1.f);
    return attenuation * (ambient_value + diffuse_value + specular_value);
  }
  else
  {
    vec3 ambient_value = lights[i].Ambience.xyz * mat_amb;
    float attenuation = global_att1 + global_att2 * light_vector_len + global_att3 * light_vector_len * light_vector_len;
    attenuation = min(1.f / attenuation, 1.f);
    return attenuation * ambient_value;
  }
}