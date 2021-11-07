//------------------------------------------------------------------------------
// File:    Light.cpp
// Author:  Ryan Buehler
// Created: Nov 5, 2021
// Desc:    Basic Light class, for an in game object that generates light
//------------------------------------------------------------------------------
#include "pch.h"
#include "Light.h"

Light::Light() noexcept :
  m_AmbientIntensity(0.1f, 0.1f, 0.1f),
  m_DiffuseIntensity(0.2f, 0.6f, 0.2f),
  m_SpecularIntensity(0.8f, 0.8f, 0.8f),
  m_LightIsActive(false)
{
}

Transform& Light::GetTransform() noexcept
{
  return m_Transform;
}

const vec3& Light::GetAmbientIntensity() const noexcept
{
  return m_AmbientIntensity;
}

const vec3& Light::GetDiffuseIntensity() const noexcept
{
  return m_DiffuseIntensity;
}

const vec3& Light::GetSpecularIntensity() const noexcept
{
  return m_SpecularIntensity;
}
