/*******************************************************************************
Shader: BlinnPhongRefract Shader [Fragment]
Author: Ryan Buehler
*******************************************************************************/
#version 460 core
#define LIGHTCOUNT 16

// TODO: Looking for a better way than macro defines
#define POINT 0
#define DIRECTION 1
#define SPOT 2

#define XNEG 0
#define XPOS 1
#define YNEG 2
#define YPOS 3
#define ZNEG 4
#define ZPOS 5

// Per Scene
uniform vec3 cam_position;

// Global Lighting Data
// WIP TODO: Convert these to a uniform block
uniform vec3 global_amb;        // Global ambience
uniform vec3 global_fog;        // Global fog intensity
uniform float global_fog_near;  // Global fog near distance
uniform float global_fog_far;   // Global fog far distance
uniform float global_att1;      // Global attenuation (constant)
uniform float global_att2;      // Global attenuation (linear)
uniform float global_att3;      // Global attenuation (quadratic)

// Per Object
// Material Data
// WIP TODO: Convert these to a uniform block
uniform vec3 mat_emit;
uniform float mat_amb;
uniform float mat_dif;
uniform float mat_spc;
uniform float mat_spc_exp;

uniform float refract_slider;
uniform float IOR_R;
uniform float IOR_G;
uniform float IOR_B;

uniform bool RefractEnabled;
uniform bool ReflectEnabled;

// Per Light Data
struct Light
{
  vec4 Position;      // Light's location in world space
  vec4 Ambience;      // Light's ambient intensity
  vec4 Diffuse;       // Light's diffuse intensity
  vec4 Specular;      // Light's specular intensity
  vec4 Direction;     // Light's direction (spot, directional lights only)
  int Type;           // Light type (point, spot, directional)
  float InnerFalloff; // Inner falloff (spot light only)
  float OuterFalloff; // Outer falloff (spot light only)
  bool IsActive;      // [T/F] The light should be factored in calculations
};

// Uniform block for Light Data
uniform LightArray
{
  Light lights[LIGHTCOUNT];
};

layout(binding=0) uniform sampler2D EnvMap_XNEG;
layout(binding=1) uniform sampler2D EnvMap_XPOS;
layout(binding=2) uniform sampler2D EnvMap_YNEG;
layout(binding=3) uniform sampler2D EnvMap_YPOS;
layout(binding=4) uniform sampler2D EnvMap_ZNEG;
layout(binding=5) uniform sampler2D EnvMap_ZPOS;

in vec4 world_position; // Fragment's world position
in vec4 world_normal;   // Fragment's world normal
in vec2 uv;             // Texture coordinates
in float ratio;

out vec4 frag_color;    // Final fragment color

vec3 calcDirectionLight(int i, vec4 view_vector); // Calculates light intensity from a directional light
vec3 calcPointLight(int i, vec4 view_vector);     // Calculates light intensity from a point light
vec3 calcSpotLight(int i, vec4 view_vector);      // Calculates light intensity from a spot light
vec3 sampleCubeMap(vec3 vEntity);
vec3 calcRefraction(vec3 V, vec3 N, float K);

void main(void)
{
  vec3 local = global_amb + mat_emit;

  // Calculate the view vector
  vec4 view_vector = vec4(cam_position, 1.f) - world_position;
  float view_vector_len = length(view_vector);
  vec4 view_vector_norm = normalize(view_vector);

  for(int i = 0; i < LIGHTCOUNT; ++i)
  {
    if(lights[i].IsActive)
    {
      switch(lights[i].Type)
      {
        // Point light
        case POINT:
          local += calcPointLight(i, view_vector_norm);
          break;
        // Directional light
        case DIRECTION:
          local += calcDirectionLight(i, view_vector_norm);
          break;
        // Spot light
        case SPOT:
          local += calcSpotLight(i, view_vector_norm);
          break;
        default:
          break;
      }
    }
  }

  // Fog Calculations
  float fog_value = (global_fog_far - view_vector_len) / (global_fog_far - global_fog_near);

  vec3 shader_value = fog_value * local + (1.f - fog_value) * global_fog;

  vec3 refract_value = vec3(0.f, 0.f, 0.f);
  if(refract_slider > 0.f)
  {
    if(RefractEnabled && ReflectEnabled)
    {
      vec3 reflect_vector = 2.f * dot(view_vector.xyz, world_normal.xyz) * world_normal.xyz - view_vector.xyz;
      vec3 refract_vectorR = calcRefraction(view_vector_norm.xyz, normalize(world_normal.xyz), 1.f / IOR_R);
      vec3 refract_vectorG = calcRefraction(view_vector_norm.xyz, normalize(world_normal.xyz), 1.f / IOR_G);
      vec3 refract_vectorB = calcRefraction(view_vector_norm.xyz, normalize(world_normal.xyz), 1.f / IOR_B);

      refract_value.r = sampleCubeMap(refract_vectorR).r;
      refract_value.g = sampleCubeMap(refract_vectorG).g;
      refract_value.b = sampleCubeMap(refract_vectorB).b;

      refract_value = mix(refract_value, sampleCubeMap(reflect_vector), ratio);
    }
    else if(ReflectEnabled)
    {
      vec3 reflect_vector = 2.f * dot(view_vector.xyz, world_normal.xyz) * world_normal.xyz - view_vector.xyz;
      refract_value = sampleCubeMap(reflect_vector);
    }
    else if(RefractEnabled)
    {
      vec3 refract_vectorR = calcRefraction(view_vector_norm.xyz, normalize(world_normal.xyz), 1.f / IOR_R);
      vec3 refract_vectorG = calcRefraction(view_vector_norm.xyz, normalize(world_normal.xyz), 1.f / IOR_G);
      vec3 refract_vectorB = calcRefraction(view_vector_norm.xyz, normalize(world_normal.xyz), 1.f / IOR_B);

      refract_value.r = sampleCubeMap(refract_vectorR).r;
      refract_value.g = sampleCubeMap(refract_vectorG).g;
      refract_value.b = sampleCubeMap(refract_vectorB).b;
    }
  }

  frag_color = vec4(mix(shader_value, refract_value, refract_slider), 1.f);
}

vec3 calcDirectionLight(int i, vec4 view_vector)
{
  // Calculate the light vector
  vec4 light_vector = normalize(-lights[i].Direction);
  
  // Calculate the half vector
  vec4 half_vector = normalize(light_vector + view_vector);

  vec3 ambient_value = lights[i].Ambience.xyz * mat_amb;
  vec3 diffuse_value = lights[i].Diffuse.xyz * mat_dif * max(dot(world_normal, light_vector), 0.f);
  vec3 specular_value = lights[i].Specular.xyz * mat_spc * pow(max(dot(world_normal, half_vector), 0.f), mat_spc_exp);

  // No light attenuation on directional lights
  return ambient_value + diffuse_value + specular_value;
}

vec3 calcPointLight(int i, vec4 view_vector)
{
  // Calculate the light vector
  vec4 light_vector = lights[i].Position - world_position;
  float light_vector_len = length(light_vector);
  vec4 light_vector_norm = normalize(light_vector);

  // Calculate the half vector
  vec4 half_vector = normalize(light_vector + view_vector);

  vec3 ambient_value = lights[i].Ambience.xyz * mat_amb;
  vec3 diffuse_value = lights[i].Diffuse.xyz * mat_dif * max(dot(world_normal, light_vector_norm), 0.f);
  vec3 specular_value = lights[i].Specular.xyz * mat_spc * pow(max(dot(world_normal, half_vector), 0.f), mat_spc_exp);

  // Factor in light attenuation
  float attenuation = global_att1 + global_att2 * light_vector_len + global_att3 * light_vector_len * light_vector_len;
  attenuation = min(1.f / attenuation, 1.f);
  return attenuation * (ambient_value + diffuse_value + specular_value);
}

vec3 calcSpotLight(int i, vec4 view_vector)
{
  // Calculate the light vector
  vec4 light_vector = lights[i].Position - world_position;
  float light_vector_len = length(light_vector);
  vec4 light_vector_norm = normalize(light_vector);

  // Calculate the difference between our light vector and the direction of the light
  float theta = dot(light_vector_norm, normalize(-lights[i].Direction));

  // Inner radius has full light
  float inner = cos(radians(lights[i].InnerFalloff));
  // Outer radius fades to nothing
  float outer = cos(radians(lights[i].OuterFalloff));

  if(theta > outer)
  {
    // Calculate the half vector
    vec4 half_vector = normalize(light_vector + view_vector);

    float falloff = 1.f - clamp((theta - inner) / (outer - inner), 0.f, 1.f);

    vec3 ambient_value = lights[i].Ambience.xyz * mat_amb;
    vec3 diffuse_value = falloff * lights[i].Diffuse.xyz * mat_dif * max(dot(world_normal, light_vector_norm), 0.f);
    vec3 specular_value = falloff * lights[i].Specular.xyz * mat_spc * pow(max(dot(world_normal, half_vector), 0.f), mat_spc_exp);

    // Factor in light attenuation
    float attenuation = global_att1 + global_att2 * light_vector_len + global_att3 * light_vector_len * light_vector_len;
    attenuation = min(1.f / attenuation, 1.f);
    return attenuation * (ambient_value + diffuse_value + specular_value);
  }
  else
  {
    // The spot light does not fall upon this fragment. Use ambient only.
    vec3 ambient_value = lights[i].Ambience.xyz * mat_amb;

    // Factor in light attenuation
    float attenuation = global_att1 + global_att2 * light_vector_len + global_att3 * light_vector_len * light_vector_len;
    attenuation = min(1.f / attenuation, 1.f);
    return attenuation * ambient_value;
  }
}

vec3 sampleCubeMap(vec3 vEntity)
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

vec3 calcRefraction(vec3 V, vec3 N, float K)
{
  if(K < 0.f)
    return vec3(0.f);
  else
    return (K * dot(N, V) - sqrt(1 - K * K * (1 - dot(N,V) * dot(N,V)))) * N - (K * V);
//  float K = 1.f - eta * eta * (1.f - dot(N, I) * dot(N, I));
//  if(K < 0.f)
//    return vec3(0.f);
//  else
//    return eta * I - (eta * dot(N, I) + sqrt(K)) * N;
}