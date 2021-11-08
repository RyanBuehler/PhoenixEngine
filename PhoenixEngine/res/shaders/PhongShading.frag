/*******************************************************************************
Shader: PhongLighting Shader [Fragment]
Author: Ryan Buehler
*******************************************************************************/
#version 460 core
#define LIGHTCOUNT 16

uniform vec3 cam_vector;

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
  bool IsActive;
  float padding[3];
};

uniform LightArray
{
  Light lights[LIGHTCOUNT];
};

in vec4 world_position;
in vec4 world_normal;

out vec4 frag_color;

void main(void)
{
  vec3 local = vec3(0.f, 0.f, 0.f);

  // Calculate the view vector
  vec4 view_vector = vec4(cam_vector, 1.f) - world_position;
  float view_vector_len = length(view_vector);
  vec4 view_vector_norm = normalize(view_vector);

  for(int i = 0; i < LIGHTCOUNT; ++i)
  {
    if(lights[i].IsActive)
    {
      // Calculate the light vector
      vec4 light_vector = lights[i].Position - world_position;
      float light_vector_len = length(light_vector);
      vec4 light_vector_norm = normalize(light_vector);

      // Calculates the reflection vector
      vec4 reflect_vector = 2.f * dot(world_normal, light_vector) * world_normal - light_vector;
      vec4 reflect_vector_norm = normalize(reflect_vector);

      vec3 ambient_value = lights[i].Ambience.xyz * mat_amb;
      vec3 diffuse_value = max(dot(world_normal, light_vector_norm), 0.f) * lights[i].Diffuse.xyz * mat_dif;
      vec3 spec_value = lights[i].Specular.xyz * mat_spc * pow(max(dot(reflect_vector_norm, view_vector_norm), 0.f), mat_spc_exp);

      float attenuation = global_att1 + global_att2 * light_vector_len + global_att3 * light_vector_len * light_vector_len;
      attenuation = min(1.f / attenuation, 1.f);

      local += global_amb + mat_emit + attenuation * (ambient_value + diffuse_value + spec_value);
    }
  }

  // Fog and Attenuation
  float fog_value = (global_fog_far - view_vector_len) / (global_fog_far - global_fog_near);

  frag_color = vec4(fog_value * local + (1.f - fog_value) * global_fog, 1.f);
}