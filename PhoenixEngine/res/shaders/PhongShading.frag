/*******************************************************************************
Shader: PhongLighting Shader [Fragment]
Author: Ryan Buehler
*******************************************************************************/
#version 460 core

uniform vec3 cam_vector;

uniform vec3 global_amb;
uniform vec3 global_fog;
uniform float global_fog_near;
uniform float global_fog_far;
uniform float global_att1;
uniform float global_att2;
uniform float global_att3;

uniform Light
{
  vec3 Pos;
  vec3 Amb;
  vec3 Dif;
  vec3 Spc;
} lights;
//uniform vec3 light_pos;
//uniform vec3 light_amb;
//uniform vec3 light_dif;
//uniform vec3 light_spc;

uniform vec3 mat_emit;
uniform float mat_amb;
uniform float mat_dif;
uniform float mat_spc;
uniform float mat_spc_exp;

in vec4 world_position;
in vec4 world_normal;

out vec3 frag_color;

void main(void)
{
  // Calculate the light vector
  vec3 light_vector = lights.Pos - world_position.xyz;
  float light_vector_len = length(light_vector);
  vec3 light_vector_norm = normalize(light_vector);

  // Calculate the view vector
  vec3 view_vector = cam_vector - world_position.xyz;
  float view_vector_len = length(view_vector);
  vec3 view_vector_norm = normalize(view_vector);

  // Calculates the reflection vector
  vec3 reflect_vector = 2.f * dot(world_normal.xyz, light_vector) * world_normal.xyz - light_vector;
  vec3 reflect_vector_norm = normalize(reflect_vector);

  vec3 ambient_value = lights.Amb * mat_amb;
  vec3 diffuse_value = max(dot(world_normal.xyz, light_vector_norm), 0.f) * lights.Dif * mat_dif; 
  vec3 spec_value = lights.Spc * mat_spc * pow(max(dot(reflect_vector_norm, view_vector_norm), 0.f), mat_spc_exp);

  // Fog and Attenuation
  float fog_value = (global_fog_far - view_vector_len) / (global_fog_far - global_fog_near);

  float attenuation = global_att1 + global_att2 * light_vector_len + global_att3 * light_vector_len * light_vector_len;
  attenuation = min(1.f / attenuation, 1.f);

  vec3 local = global_amb + mat_emit + attenuation * (ambient_value + diffuse_value + spec_value);

  frag_color = fog_value * local + (1.f - fog_value) * global_fog;
}