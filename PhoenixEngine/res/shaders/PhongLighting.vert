/*******************************************************************************
Shader: PhongLighting Shader [Vertex]
Author: Ryan Buehler
*******************************************************************************/
#version 460 core

// Per Scene
uniform mat4 pers_matrix;
uniform mat4 view_matrix;
uniform vec3 cam_vector;

uniform vec3 global_amb;
uniform vec3 global_fog;
uniform float global_fog_near;
uniform float global_fog_far;
uniform float global_att1;
uniform float global_att2;
uniform float global_att3;

uniform vec3 light_pos;
uniform vec3 light_amb;
uniform vec3 light_dif;
uniform vec3 light_spc;

// Per Object
uniform mat4 model_matrix;

uniform vec3 mat_emit;
uniform float mat_amb;
uniform float mat_dif;
uniform float mat_spc;
uniform float mat_spc_exp;

in layout(location = 0) vec3 position;
in layout(location = 1) vec3 normal;

out vec3 frag_color_vert;

void main(void)
{
  // Calculate the world position of the vertex
  gl_Position = pers_matrix * view_matrix * model_matrix * vec4(position, 1.f);

  // Calculate the light vector
  vec3 light_vector = light_pos - gl_Position.xyz;
  float light_vector_len = length(light_vector);
  vec3 light_vector_norm = normalize(light_vector);

  // Calculate the view vector
  vec3 view_vector = cam_vector - gl_Position.xyz;
  float view_vector_len = length(view_vector);
  vec3 view_vector_norm = normalize(view_vector);

  // Calculates the reflection vector
  vec4 surface_normal = pers_matrix * view_matrix * model_matrix * vec4(normal,1);
  vec3 reflect_vector = 2.f * dot(surface_normal.xyz, light_vector) * surface_normal.xyz - light_vector;
  vec3 reflect_vector_norm = normalize(reflect_vector);

  vec3 ambient_value = light_amb * mat_amb;
  vec3 diffuse_value = max(dot(surface_normal.xyz, light_vector_norm), 0.f) * light_dif * mat_dif; 
  vec3 spec_value = { 0.f, 0.f, 0.f };
  //vec3 spec_value = light_spc * mat_spc * pow(max(dot(reflect_vector_norm, view_vector_norm), 0.f), mat_spc_exp);

  // Fog and Attenuation
  float fog_value = (global_fog_far - view_vector_len) / (global_fog_far - global_fog_near);

  float attenuation = global_att1 + global_att2 * light_vector_len + global_att3 * light_vector_len * light_vector_len;
  attenuation = min(1.f / attenuation, 1.f);

  vec3 local = mat_emit + attenuation * (ambient_value + diffuse_value + spec_value);

  frag_color_vert = local;
  //frag_color_vert = fog_value * local + (1.f - fog_value) * global_fog;
}