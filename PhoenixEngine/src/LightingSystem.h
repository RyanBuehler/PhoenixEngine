//------------------------------------------------------------------------------
// File:    LightingSystem.h
// Author:  Ryan Buehler
// Created: 11/02/21
// Desc:    The Lighting System (currently Phong)
//------------------------------------------------------------------------------
#pragma once
#include "GraphicsCommon.h"

class Lighting
{
public:

  Lighting() noexcept;
  ~Lighting() = default;
  Lighting(const Lighting&) = delete;
  Lighting& operator=(const Lighting&) = delete;
  Lighting(Lighting&&) = delete;
  Lighting& operator=(Lighting&&) = delete;

  void SetGlobalFog(const vec3& fogValue) noexcept;
  void SetFogNear(float fogNear) noexcept;
  void SetFogFar(float fogFar) noexcept;

  void SetAttenuation(const float attenuation[3]) noexcept;

private:
  vec3 m_GlobalFog; // Global fog intensity
  float m_FogNear;  // Fog near factor
  float m_FogFar;   // Fog far factor

  float m_Attenuation[3];
};
