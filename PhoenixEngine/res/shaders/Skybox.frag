/*******************************************************************************
Shader: Diffuse Shader [Fragment]
Author: Ryan Buehler
*******************************************************************************/
#version 460 core

uniform samplerCube skybox;

in vec3 texcoords;

out vec4 frag_color;

void main(void)
{
  //frag_color = vec4(1.f, 0.f, 0.f, 1.f);
  frag_color = texture(skybox, texcoords);
}