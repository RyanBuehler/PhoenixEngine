/*******************************************************************************
Shader: Reflection Shader [Fragment]
Author: Ryan Buehler
*******************************************************************************/
#version 460 core

#define XNEG 0
#define XPOS 1
#define YNEG 2
#define YPOS 3
#define ZNEG 4
#define ZPOS 5

uniform vec3 cam_position;

layout(binding=0) uniform sampler2D EnvMap_XNEG;
layout(binding=1) uniform sampler2D EnvMap_XPOS;
layout(binding=2) uniform sampler2D EnvMap_YNEG;
layout(binding=3) uniform sampler2D EnvMap_YPOS;
layout(binding=4) uniform sampler2D EnvMap_ZNEG;
layout(binding=5) uniform sampler2D EnvMap_ZPOS;

in vec4 world_position;
in vec4 world_normal;

out vec4 frag_color;

vec3 SampleCubeMap(vec3 vEntity);

void main(void)
{
  vec3 view_vector = normalize(cam_position - world_position.xyz);
  vec3 reflect_vector = 2.f * dot(view_vector, world_normal.xyz) * world_normal.xyz - view_vector;

  //frag_color = world_normal;
  frag_color = vec4(SampleCubeMap(reflect_vector), 1.f);
}

vec3 SampleCubeMap(vec3 vEntity)
{
  vec3 absVec = abs(vEntity);
  vec2 uv = vec2(0.0);
  int side = 6;

  // +-X
  if(absVec.x >= absVec.y && absVec.x >= absVec.z)
  {
    vEntity *= 1 / absVec.x;
    if(vEntity.x < 0.f)
    {
      uv.x = -vEntity.z;
      side = XPOS;
    }
    else
    {
      uv.x = vEntity.z;
      side = XNEG;
    }
    uv.y = vEntity.y;
  }
  // +-Y
  else if(absVec.y >= absVec.x && absVec.y >= absVec.z)
  {
    vEntity *= 1 / absVec.y;
    if(vEntity.y < 0.f )
    {
      uv.y = vEntity.z;
      side = YNEG;
    }
    else
    {
      uv.y = -vEntity.z;
      side = YPOS;
    }
    uv.x = -vEntity.x;
  }
  // +-Z
  else
  {
    vEntity *= 1 / absVec.z;
    if(vEntity.z < 0.f )
    {
      uv.x = vEntity.x;
      side = ZPOS;
    }
    else
    {
      uv.x = -vEntity.x;
      side = ZNEG;
    }
    uv.y = vEntity.y;
  }

  // Convert range from -1 to 1 to 0 to 1
  uv = (uv + vec2(1.0f) ) * 0.5f;

  switch(side)
  {
  case XNEG:
    //return vec3(1.f, 1.f, 0.f); // Yellow
    return texture(EnvMap_XNEG, uv).rgb;
  case XPOS:
    //return vec3(1.f, 0.f, 0.f); // Red
    return texture(EnvMap_XPOS, uv).rgb;
  case YNEG:
    //return vec3(1.f, 0.f, 1.f); // Purple
    return texture(EnvMap_YNEG, uv).rgb;
  case YPOS:
    //return vec3(0.f, 1.f, 0.f); // Green
    return texture(EnvMap_YPOS, uv).rgb;
  case ZNEG:
    //return vec3(0.f, 0.f, 1.f); // Blue
    return texture(EnvMap_ZNEG, uv).rgb;
  case ZPOS:
    //return vec3(0.f, 1.f, 1.f); // Cyan
    return texture(EnvMap_ZPOS, uv).rgb;
  default:
    return vec3(1.f, 0.f, 0.f);
    //break;
  }
}