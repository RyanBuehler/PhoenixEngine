//------------------------------------------------------------------------------
// File:    LightingSystem.cpp
// Author:  Ryan Buehler
// Created: 11/02/21
// Desc:    The Lighting System (currently Phong)
//------------------------------------------------------------------------------
#include "pch.h"
#include "LightingSystem.h"

//TODO: set clamps on the scene data based on reasonable values

LightingSystem::LightingSystem() noexcept :
  m_RegisteredLights(),
  m_LightingIsDirty(true),
  m_ActiveLightLimit(MAX_DYNAMIC_LIGHTS)
{
  Log::Trace("Lighting System initialized");
}

LightingSystem::~LightingSystem()
{
  Log::Trace("Lighting system destructed");
}

bool LightingSystem::RegisterLight(Light* lightPtr) noexcept
{
  if (getFreeLightCount() < 1)
  {
    Log::Warn("Attempting to register light, but maximum already reached");
    return false;
  }

  // Check for light redundancies
  for (Light* ptr : m_RegisteredLights)
  {
    if (ptr == lightPtr)
    {
      Log::Warn("Attempting to register an already registered light");
      return false;
    }
  }

  // Set the index 
  unsigned index = LIGHT_ERROR;
  for (unsigned i = 0; i < MAX_DYNAMIC_LIGHTS; ++i)
  {
    if (m_RegisteredLights[i] == nullptr)
    {
      index = i;
    }
  }

  // Since the count was checked earlier, one light should exist
  if (index == LIGHT_ERROR)
  {
    Log::Error("No free light slot exists, when one should");
    return false;
  }

  m_RegisteredLights[index] = lightPtr;
  Log::Trace("Light registered in slot: " + index);
  return true;
}

bool LightingSystem::DerigesterLight(Light* lightPtr) noexcept
{
  unsigned index = LIGHT_ERROR;
  for (unsigned i = 0; i < MAX_DYNAMIC_LIGHTS; ++i)
  {
    if (m_RegisteredLights[i] == lightPtr)
    {
      index = i;
    }
  }

  if (index == LIGHT_ERROR)
  {
    Log::Warn("Attempting to deregister light. Light not found");
    return false;
  }

  Log::Trace("Light deregistered from slot: " + index);
  return true;
}

bool LightingSystem::FreeLightSlotExists() const noexcept
{
  for (Light* ptr : m_RegisteredLights)
  {
    if (ptr == nullptr)
    {
      return true;
    }
  }

  return false;
}

const LightingSystem::GlobalLightingData& LightingSystem::GetGlobalLightingData() const noexcept
{
  return m_GlobalLightingData;
}

void LightingSystem::SetGlobalAmbientIntensity(const vec3& globalAmbientIntensity) noexcept
{
  m_GlobalLightingData.AmbientIntensity = globalAmbientIntensity;
}

void LightingSystem::SetGlobalFogIntensity(const vec3& globalFogIntensity) noexcept
{
  m_GlobalLightingData.FogIntensity = globalFogIntensity;
}

void LightingSystem::SetFogNearDistance(float fogNear) noexcept
{
  m_GlobalLightingData.FogNear = fogNear;
}

void LightingSystem::SetFogFarDistance(float fogFar) noexcept
{
  m_GlobalLightingData.FogFar = fogFar;
}


void LightingSystem::SetLightAttenuation(float constant, float linear, float quadratic) noexcept
{
  m_GlobalLightingData.AttConstant = constant;
  m_GlobalLightingData.AttLinear = linear;
  m_GlobalLightingData.AttQuadratic = quadratic;
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
  for (Light* ptr : m_RegisteredLights)
  {
    if (ptr == nullptr)
    {
      ++count;
    }
  }
  return count;
}
