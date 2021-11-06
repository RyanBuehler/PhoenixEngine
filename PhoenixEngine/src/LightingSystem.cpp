//------------------------------------------------------------------------------
// File:    LightingSystem.cpp
// Author:  Ryan Buehler
// Created: 11/02/21
// Desc:    The Lighting System (currently Phong)
//------------------------------------------------------------------------------
#include "pch.h"
#include "LightingSystem.h"

Lighting::Lighting() noexcept :
  m_GlobalFog({ 0.1f, 0.1f, 0.1f }),
  m_FogNear(1.f),
  m_FogFar(20.f),
  m_Attenuation()
{
}

void Lighting::SetGlobalFog(const vec3& fogValue) noexcept
{
  m_GlobalFog = fogValue;
}

void Lighting::SetFogNear(float fogNear) noexcept
{
  m_FogNear = fogNear;
}

void Lighting::SetFogFar(float fogFar) noexcept
{
  m_FogFar = fogFar;
}

void Lighting::SetAttenuation(const float attenuation[3]) noexcept
{
  m_Attenuation[0] = attenuation[0];
  m_Attenuation[1] = attenuation[1];
  m_Attenuation[2] = attenuation[2];
}
