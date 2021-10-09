/*******************************************************************************
Shader: Debug Shader [Vertex]
Author: Ryan Buehler
*******************************************************************************/
#version 460 core

uniform mat4 pers_matrix;
uniform mat4 view_matrix;
uniform mat4 model_matrix;

in layout(location = 0) vec3 position;
in layout(location = 1) vec3 color;

out vec3 frag_diffuse;

void main(void)
{
  frag_diffuse = color;
  gl_Position = pers_matrix * view_matrix * model_matrix * vec4(position, 1.f);
  //gl_Position = vec4(position, 1.f);
}