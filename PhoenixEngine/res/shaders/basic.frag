/*******************************************************************************
Shader: Basic Shader [Fragment]
Author: Ryan Buehler
*******************************************************************************/
#version 460 core

in vec3 frag_normal;
in vec3 frag_diffuse;

out vec3 frag_color;

void main(void)
{
  vec3 vLight = vec3(0.f, 0.f, 1.f);
  float ndot = max(dot(normalize(frag_normal), normalize(vLight)), 0.0f);
  //frag_color = frag_diffuse * ndot;
  frag_color = frag_diffuse;
}