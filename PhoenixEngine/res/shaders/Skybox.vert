/*******************************************************************************
Shader: Diffuse Shader [Vertex]
Author: Ryan Buehler
*******************************************************************************/
#version 460 core

uniform mat4 pers_matrix;
uniform mat4 view_matrix;

in layout(location = 0) vec3 position;

out vec3 texcoords;

void main(void)
{
  texcoords = position;
  vec4 pos = pers_matrix * view_matrix * vec4(position, 1.f);
  gl_Position = pos.xyww;
}