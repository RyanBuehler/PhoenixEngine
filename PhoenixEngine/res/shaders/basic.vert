/*******************************************************************************
Shader: Basic Shader [Vertex]
Author: Ryan Buehler
*******************************************************************************/
#version 460

layout(location = 0) in vec3 position_object;

out vec4 position_world;

void main(void)
{
  position_world = vec4(position_object, 1.f);

  gl_Position = position_world;
}