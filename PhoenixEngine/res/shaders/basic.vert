/*******************************************************************************
Shader: Basic Shader [Vertex]
Author: Ryan Buehler
*******************************************************************************/
#version 460
in vec4 position;

uniform mat4 pers_matrix;
uniform mat4 view_matrix;
uniform mat4 model_matrix;

out vec4 world_position;

void main(void)
{
  world_position = model_matrix * position;
  gl_Position = pers_matrix * view_matrix * world_position;
}