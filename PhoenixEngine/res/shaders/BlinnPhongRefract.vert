/*******************************************************************************
Shader: BlinnPhongRefract Shader [Vertex]
Author: Ryan Buehler
*******************************************************************************/
#version 460 core

// Per Scene
uniform mat4 pers_matrix;
uniform mat4 view_matrix;

// Per Object
uniform mat4 model_matrix;

uniform vec3 cam_position;
uniform float IOR_G;

in layout(location = 0) vec3 position;  // Vertex position
in layout(location = 1) vec3 normal;    // Vertex normal
in layout(location = 2) vec2 texcoord;  // UV Coordinate

out vec4 world_position;  // Interpolated world position
out vec4 world_normal;    // Interpolated world normal
out vec2 uv;              // Interpolated uv texture coordinate
out float ratio;

void main(void)
{
  float Eta = 1.f / IOR_G;
  float F = ((1.0f - Eta) * (1.0f - Eta)) / ((1.0f + Eta) * (1.0f + Eta));
  // Calculate the world position of the vertex
  world_position = model_matrix * vec4(position, 1.f);

  // Calculate the world position of the normal
  world_normal = normalize(transpose(inverse(model_matrix)) * vec4(normal, 0));

  ratio = F + (1.f - F) * pow((1.f - dot(-normalize(world_position.xyz - cam_position), world_normal.xyz)), 3.f);

  // Pass the texture coordinates
  uv = texcoord;

  gl_Position = pers_matrix * view_matrix * world_position;
}