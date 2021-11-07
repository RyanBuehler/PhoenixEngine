/*******************************************************************************
Shader: PhongLighting Shader [Fragment]
Author: Ryan Buehler
*******************************************************************************/
#version 460 core

in vec3 frag_color_vert;

out vec3 frag_color;

void main(void)
{
  frag_color = frag_color_vert;
}