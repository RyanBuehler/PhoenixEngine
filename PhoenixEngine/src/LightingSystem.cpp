//------------------------------------------------------------------------------
// File:    LightingSystem.cpp
// Author:  Ryan Buehler
// Created: 11/02/21
// Desc:    The Lighting System (currently Phong)
//------------------------------------------------------------------------------
#include "pch.h"
#include "LightingSystem.h"

LightingSystem::LightingSystem() noexcept :
  m_RegisteredLights()
{
  Log::trace("Lighting System initialized");
}

LightingSystem::~LightingSystem()
{
  Log::trace("Lighting system destructed");
}

bool LightingSystem::RegisterLight(Light* LightPtr) noexcept
{
  if (getFreeLightCount() < 1)
  {
    Log::warn("Attempting to register light, but maximum already reached");
    return false;
  }

  // Check for light redundancies
  for (const Light* lightPtr : m_RegisteredLights)
  {
    if (lightPtr == LightPtr)
    {
      Log::warn("Attempting to register an already registered light");
      return false;
    }
  }

  // Set the index 
  unsigned index = Error::INVALID_INDEX;
  for (unsigned i = 0; i < MAX_DYNAMIC_LIGHTS; ++i)
  {
    if (m_RegisteredLights[i] == nullptr)
    {
      index = i;
    }
  }

  // Since the count was checked earlier, one light should exist
  if (index == Error::INVALID_INDEX)
  {
    Log::error("No free light slot exists, when one should");
    return false;
  }

  m_RegisteredLights[index] = LightPtr;
  Log::trace("Light registered in slot: " + std::to_string(index));
  return true;
}

bool LightingSystem::DeRegisterLight(const Light* LightPtr) const noexcept
{
  unsigned index = Error::INVALID_INDEX;
  for (unsigned i = 0; i < MAX_DYNAMIC_LIGHTS; ++i)
  {
    if (m_RegisteredLights[i] == LightPtr)
    {
      index = i;
    }
  }

  if (index == Error::INVALID_INDEX)
  {
    Log::warn("Attempting to de-register light. Light not found");
    return false;
  }

  Log::trace("Light de-registered from slot: " + std::to_string(index));
  return true;
}

bool LightingSystem::FreeLightSlotExists() const noexcept
{
  return std::ranges::any_of(m_RegisteredLights, [](const Light* LightPtr) { return LightPtr == nullptr; });
}

const LightingSystem::GlobalLightingData& LightingSystem::GetGlobalLightingData() const noexcept
{
  return m_GlobalLightingData;
}

void LightingSystem::SetGlobalAmbientIntensity(const vec3& globalAmbientIntensity) noexcept
{
  m_GlobalLightingData.AmbientIntensity = globalAmbientIntensity;
}

void LightingSystem::SetGlobalFogIntensity(const vec3& GlobalFogIntensity) noexcept
{
  m_GlobalLightingData.FogIntensity = GlobalFogIntensity;
}

void LightingSystem::SetFogNearDistance(float FogNear) noexcept
{
  m_GlobalLightingData.FogNear = FogNear;
}

void LightingSystem::SetFogFarDistance(float FogFar) noexcept
{
  m_GlobalLightingData.FogFar = FogFar;
}


void LightingSystem::SetLightAttenuation(float Constant, float Linear, float Quadratic) noexcept
{
  m_GlobalLightingData.AttConstant = Constant;
  m_GlobalLightingData.AttLinear = Linear;
  m_GlobalLightingData.AttQuadratic = Quadratic;
}

const vec3& LightingSystem::GetGlobalAmbientIntensity() const noexcept
{
  return m_GlobalLightingData.AmbientIntensity;
}

const vec3& LightingSystem::GetGlobalFogIntensity() const noexcept
{
  return m_GlobalLightingData.FogIntensity;
}

float LightingSystem::GetFogNearDistance() const noexcept
{
  return m_GlobalLightingData.FogNear;
}

float LightingSystem::GetFogFarDistance() const noexcept
{
  return m_GlobalLightingData.FogFar;
}

float LightingSystem::GetLightAttenuationConstant() const noexcept
{
  return m_GlobalLightingData.AttConstant;
}

float LightingSystem::GetLightAttenuationLinear() const noexcept
{
  return m_GlobalLightingData.AttLinear;
}

float LightingSystem::GetLightAttenuationQuadratic() const noexcept
{
  return m_GlobalLightingData.AttQuadratic;
}

unsigned LightingSystem::getFreeLightCount() const noexcept
{
  return MAX_DYNAMIC_LIGHTS - getActiveLightCount();
}

unsigned LightingSystem::getActiveLightCount() const noexcept
{
  unsigned count = 0;
  for (const Light* lightPtr : m_RegisteredLights)
  {
    if (lightPtr == nullptr)
    {
      ++count;
    }
  }
  return count;
}
