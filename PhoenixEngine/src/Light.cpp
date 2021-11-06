//------------------------------------------------------------------------------
// File:    Light.cpp
// Author:  Ryan Buehler
// Created: Nov 5, 2021
// Desc:    Basic Light class, for an in game object that generates light
//------------------------------------------------------------------------------
#include "pch.h"
#include "Light.h"

Light::Light() noexcept :
  m_AmbientIntensity(0.f, 0.f, 0.f),
  m_DiffuseIntensity(0.f, 0.f, 0.f),
  m_SpecularIntensity(0.f, 0.f, 0.f),
  m_LightIsActive(false)
{
}
