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

out vec4 world_position;
out vec4 world_normal;

void main(void)
{
  // Calculate the world position of the vertex
  world_position = model_matrix * vec4(position, 1.f);

  // Calculate the world position of the normal
  world_normal = model_matrix * vec4(normal, 1.f);

  gl_Position = pers_matrix * view_matrix * model_matrix * vec4(position, 1.f);
}