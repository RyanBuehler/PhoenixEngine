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
  constexpr static int POINT_LIGHT = 0;
  constexpr static int DIRECTION_LIGHT = 1;
  constexpr static int SPOT_LIGHT = 2;

  struct Data
  {
    Data() :
      Position(vec4(0.f, 0.f, 0.f, 1.f)),
      AmbientIntensity(vec4(0.f, 0.f, 0.f, 1.f)),
      DiffuseIntensity(vec4(0.8f, 0.8f, 0.8f, 1.f)),
      SpecularIntensity(vec4(1.f, 1.0f, 1.0f, 1.f)),
      Direction(vec4(0.f, -1.f, 0.f, 0.f)),
      Type(POINT_LIGHT),
      InnerFalloff(35.f),
      OuterFalloff(37.f),
      bIsActive(false)
    {}

    vec4 Position;          // The light's position in world space
    vec4 AmbientIntensity;  // The light's ambient intensity value
    vec4 DiffuseIntensity;  // The light's diffuse intensity value
    vec4 SpecularIntensity; // The light's specular intensity value
    vec4 Direction;         // The light's direction if spot/directional
    int Type;               // The type of light (point, spot, directional)
    float InnerFalloff;     // The degrees at which spot lights fall off
    float OuterFalloff;     // The degrees at which spot lights fall off
    bool bIsActive;          // Is the light currently active?
  };
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
  /// Gets the Light Data for the GPU
  /// </summary>
  /// <returns>[Const] Reference to the light's data</returns>
  const Data& GetData() noexcept;

  /// <summary>
  /// Gets the light's transform
  /// </summary>
  /// <returns>A reference to the light's transform</returns>
  Transform& GetTransform() noexcept;

private:
  Transform m_Transform;    // The light's transform
  Data m_Data;              // Light data to be passed to the GPU
#ifdef _IMGUI
  friend class ImGuiManager;
#endif
};
