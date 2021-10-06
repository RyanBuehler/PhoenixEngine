/*******************************************************************************
Shader: Basic Shader [Vertex]
Author: Ryan Buehler
*******************************************************************************/
#version 460 core

uniform mat4 pers_matrix;
uniform mat4 view_matrix;
uniform mat4 model_matrix;

in layout(location = 0) vec3 position;
in layout(location = 1) vec3 normal;

//out vec4 world_position;
out vec3 frag_normal;
out vec3 frag_diffuse;

void main(void)
{
  frag_diffuse = vec3(11.f / 255.f, 222.f / 255.f, 230.f / 255.f);
  frag_normal = normal;
  gl_Position = pers_matrix * view_matrix * model_matrix * vec4(position, 1.f);
}