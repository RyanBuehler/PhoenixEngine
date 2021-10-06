/*******************************************************************************
Shader: Basic Shader [Fragment]
Author: Ryan Buehler
*******************************************************************************/
#version 460 core

in vec3 frag_diffuse;

out vec3 frag_color;

void main(void)
{
  frag_color = frag_diffuse;
}