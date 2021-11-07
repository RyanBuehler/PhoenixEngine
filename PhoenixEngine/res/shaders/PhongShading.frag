/*******************************************************************************
Shader: PhongLighting Shader [Fragment]
Author: Ryan Buehler
*******************************************************************************/
#version 460 core

in vec3 frag_color;

out vec3 frag_color;

void main(void)
{
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
  //vec4 surface_normal = model_matrix * vec4(normal,1);
  vec3 reflect_vector = 2.f * dot(surface_normal.xyz, light_vector) * surface_normal.xyz - light_vector;
  vec3 reflect_vector_norm = normalize(reflect_vector);

  vec3 ambient_value = light_amb * mat_amb;
  vec3 diffuse_value = max(dot(surface_normal.xyz, light_vector_norm), 0.f) * light_dif * mat_dif; 
  vec3 spec_value = light_spc * mat_spc * pow(max(dot(reflect_vector_norm, view_vector_norm), 0.f), mat_spc_exp);

  // Fog and Attenuation
  float fog_value = (global_fog_far - view_vector_len) / (global_fog_far - global_fog_near);

  float attenuation = global_att1 + global_att2 * light_vector_len + global_att3 * light_vector_len * light_vector_len;
  attenuation = min(1.f / attenuation, 1.f);

  vec3 local = global_amb + mat_emit + attenuation * (ambient_value + diffuse_value + spec_value);

  frag_color = fog_value * local + (1.f - fog_value) * global_fog;
}