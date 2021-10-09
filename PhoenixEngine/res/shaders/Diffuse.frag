/*******************************************************************************
Shader: Diffuse Shader [Fragment]
Author: Ryan Buehler
*******************************************************************************/
#version 460 core

in vec3 frag_normal;
in vec3 frag_diffuse;
in vec3 frag_ambient;

out vec3 frag_color;

void main(void)
{
  frag_color = frag_ambient;
  vec3 vLight = vec3(0.f, 2.f, 2.f);
  float ndot = max(dot(normalize(frag_normal), normalize(vLight)), 0.0f);
  frag_color += frag_diffuse * ndot;
}