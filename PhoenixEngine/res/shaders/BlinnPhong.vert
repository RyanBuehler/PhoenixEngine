/*******************************************************************************
Shader: BlinnPhong Shader [Vertex]
Author: Ryan Buehler
*******************************************************************************/
#version 460 core

// Per Scene
uniform mat4 pers_matrix;
uniform mat4 view_matrix;

// Per Object
uniform mat4 model_matrix;

in layout(location = 0) vec3 position;  // Vertex position
in layout(location = 1) vec3 normal;    // Vertex normal
in layout(location = 2) vec2 texcoord;  // UV Coordinate

out vec4 world_position;  // Interpolated world position
out vec4 world_normal;    // Interpolated world normal
out vec2 uv;              // Interpolated uv texture coordinate

void main(void)
{
  // Calculate the world position of the vertex
  world_position = model_matrix * vec4(position, 1.f);

  // Calculate the world position of the normal
  world_normal = normalize(transpose(inverse(model_matrix)) * vec4(normal, 0));

  // Pass the texture coordinates
  uv = texcoord;

  gl_Position = pers_matrix * view_matrix * world_position;
}