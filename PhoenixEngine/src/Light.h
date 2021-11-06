//------------------------------------------------------------------------------
// File:    Light.h
// Author:  Ryan Buehler
// Created: Nov 5, 2021
// Desc:    Basic Light class, for an in game object that generates light
//------------------------------------------------------------------------------
#pragma once
#include "GraphicsCommon.h"

class Light
{
public:

  Light() noexcept;
  ~Light() = default;
  Light(const Light&) = delete;
  Light& operator=(const Light&) = delete;
  Light(Light&&) = delete;
  Light& operator=(Light&&) = delete;

private:
  vec3 m_AmbientIntensity;
  vec3 m_DiffuseIntensity;
  vec3 m_SpecularIntensity;

  bool m_LightIsActive;
};
