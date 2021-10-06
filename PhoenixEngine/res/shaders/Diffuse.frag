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
  vec3 vLight = vec3(0.f, 2.f, 2.f);
  float ndot = max(dot(normalize(frag_normal), normalize(vLight)), 0.0f);
  frag_color = frag_diffuse * ndot;
  frag_color.x = max(frag_color.x, 0.2f);
  frag_color.y = max(frag_color.y, 0.1f);
  frag_color.z = max(frag_color.z, 0.2f);
  //frag_color = frag_diffuse;
}