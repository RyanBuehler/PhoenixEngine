/*******************************************************************************
Shader: Reflection Shader [Vertex]
Author: Ryan Buehler
*******************************************************************************/
#version 460 core

uniform mat4 pers_matrix;
uniform mat4 view_matrix;
uniform mat4 model_matrix;

in layout(location = 0) vec3 position;
in layout(location = 1) vec3 normal;

out vec4 world_position;
out vec4 world_normal;

void main(void)
{
  world_position = model_matrix * vec4(position, 1.f);

  world_normal = normalize(transpose(inverse(model_matrix)) * vec4(normal, 0));

  gl_Position = pers_matrix * view_matrix * world_position;
}