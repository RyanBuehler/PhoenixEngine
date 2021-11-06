//------------------------------------------------------------------------------
// File:    Light.h
// Author:  Ryan Buehler
// Created: Nov 5, 2021
// Desc:    Basic Light class, for an in game object that generates light
//------------------------------------------------------------------------------
#pragma once
#include "GraphicsCommon.h"
#include "Transform.h"

class Light
{
public:
  /// <summary>
  /// Light Constructor
  /// </summary>
  Light() noexcept;

  // Rule of 5
  ~Light() = default;
  Light(const Light&) = delete;
  Light& operator=(const Light&) = delete;
  Light(Light&&) = delete;
  Light& operator=(Light&&) = delete;

  /// <summary>
  /// Gets the light's transform
  /// </summary>
  /// <returns>A reference to the light's transform</returns>
  Transform& GetTransform() noexcept;

private:
  Transform m_Transform;    // The light's transform

  vec3 m_AmbientIntensity;  // The light's ambient intensity value
  vec3 m_DiffuseIntensity;  // The light's diffuse intensity value
  vec3 m_SpecularIntensity; // The light's specular intensity value

  bool m_LightIsActive;     // The light is active or not
};
