/*******************************************************************************
Shader: Basic Shader [Fragment]
Author: Ryan Buehler
*******************************************************************************/
#version 460

in vec4 position_world;

out vec4 color_frag;

void main(void)
{
  color_frag = vec4(1.f, 0.f, 0.f, 1.f);
}