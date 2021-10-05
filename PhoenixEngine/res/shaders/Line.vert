/*******************************************************************************
Shader: Basic Shader [Vertex]
Author: Ryan Buehler
*******************************************************************************/
#version 460 core

uniform mat4 pers_matrix;
uniform mat4 view_matrix;

in layout(location = 0) vec3 position;

out vec3 frag_diffuse;

void main(void)
{
  frag_diffuse = vec3(1.f, 0.f, 0.f);
  gl_Position = pers_matrix * view_matrix * vec4(position, 1.f);
  //gl_Position = vec4(position, 1.f);
}