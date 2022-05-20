//------------------------------------------------------------------------------
// File:    LightingSystem.h
// Author:  Ryan Buehler
// Created: 11/02/21
// Desc:    The Lighting System (currently Phong)
//------------------------------------------------------------------------------
#pragma once
#include "GraphicsCommon.h"

// Forward declaration
class Light;

class LightingSystem
{
public:
  constexpr static unsigned MAX_DYNAMIC_LIGHTS = 16;

  struct GlobalLightingData
  {
    GlobalLightingData() noexcept :
      AmbientIntensity({ 0.01f, 0.01f, 0.01f }),
      FogIntensity({ 0.05f, 0.05f, 0.05f }),
      FogNear(1.f),
      FogFar(50.f),
      AttConstant(0.4f),
      AttLinear(0.333f),
      AttQuadratic(0.f)
    {}

    vec3 AmbientIntensity;        // Global Ambient intensity
    vec3 FogIntensity;            // Global fog intensity
    float FogNear;                // Fog near factor
    float FogFar;                 // Fog far factor
    float AttConstant;            // Constant Attenuation Factor
    float AttLinear;              // Linear Attenuation Coefficient
    float AttQuadratic;           // Quadratic Attenuation Coefficient
  };

  /// <summary>
  /// LightingSystem constructor
  /// </summary>
  LightingSystem() noexcept;
  ~LightingSystem();

  // Rule of 5
  LightingSystem(const LightingSystem&) = delete;
  LightingSystem& operator=(const LightingSystem&) = delete;
  LightingSystem(LightingSystem&&) = delete;
  LightingSystem& operator=(LightingSystem&&) = delete;

  /// <summary>
  /// Attempts to register a scene light into the active lighting system
  /// </summary>
  /// <param name="LightPtr"></param>
  /// <returns>Whether the registration was a success</returns>
  bool RegisterLight(Light* LightPtr) noexcept;

  /// <summary>
  /// De-registers a light by pointer
  /// </summary>
  /// <param name="LightPtr">A pointer to the registered light</param>
  /// <returns>T/F of Success or Failure</returns>
  bool DeRegisterLight(const Light* LightPtr) const noexcept;

  /// <summary>
  /// Checks if a free light slot exists
  /// </summary>
  /// <returns>T/F if the light slot exists</returns>
  [[nodiscard]] bool FreeLightSlotExists() const noexcept;

  /// <summary>
  /// Gets the Global Lighting Data for the scene
  /// </summary>
  /// <returns>[Const] Reference to the Global Lighting Data</returns>
  [[nodiscard]] const GlobalLightingData& GetGlobalLightingData() const noexcept;

  /// <summary>
  /// Sets the Global Ambient Intensity for the scene
  /// </summary>
  /// <param name="globalAmbientIntensity">The light intensity for the global ambience</param>
  void SetGlobalAmbientIntensity(const vec3& globalAmbientIntensity) noexcept;

  /// <summary>
  /// Sets the Global Fog Intensity
  /// </summary>
  /// <param name="GlobalFogIntensity">The light intensity for the fog</param>
  void SetGlobalFogIntensity(const vec3& GlobalFogIntensity) noexcept;

  /// <summary>
  /// Sets the near value for fog calculations
  /// </summary>
  /// <param name="FogNear">The near distance for fog calculations</param>
  void SetFogNearDistance(float FogNear) noexcept;

  /// <summary>
  /// Sets the far value for fog calculations
  /// </summary>
  /// <param name="FogFar">The far distance for fog calculations</param>
  void SetFogFarDistance(float FogFar) noexcept;

  /// <summary>
  /// Sets the light attenuation for the scene
  /// </summary>
  /// <param name="Constant">The constant factor in the attenuation</param>
  /// <param name="Linear">The linear coefficient in the attenuation</param>
  /// <param name="Quadratic">The quadratic coefficient in the attenuation</param>
  void SetLightAttenuation(float Constant, float Linear, float Quadratic) noexcept;

  /// <summary>
  /// Gets the Global Ambient Intensity
  /// </summary>
  /// <returns>[const] The global ambient intensity value</returns>
  [[nodiscard]] const vec3& GetGlobalAmbientIntensity() const noexcept;

  /// <summary>
  /// Gets the Global Fog Intensity
  /// </summary>
  /// <returns>[const] The global fog intensity value</returns>
  [[nodiscard]] const vec3& GetGlobalFogIntensity() const noexcept;

  /// <summary>
  /// Gets the fog near distance
  /// </summary>
  /// <returns>The fog near distance value</returns>
  [[nodiscard]] float GetFogNearDistance() const noexcept;

  /// <summary>
  /// Gets the fog far distance
  /// </summary>
  /// <returns>The fog far distance value</returns>
  [[nodiscard]] float GetFogFarDistance() const noexcept;

  [[nodiscard]] float GetLightAttenuationConstant() const noexcept;
  [[nodiscard]] float GetLightAttenuationLinear() const noexcept;
  [[nodiscard]] float GetLightAttenuationQuadratic() const noexcept;

private:
  /// <summary>
  /// Helper function to get the number of active lights
  /// </summary>
  /// <returns>The active light count</returns>
  [[nodiscard]] unsigned getActiveLightCount() const noexcept;

  /// <summary>
  /// Helper function that gets the number of lights free to register
  /// </summary>
  /// <returns>The count of light slots still open</returns>
  [[nodiscard]] unsigned getFreeLightCount() const noexcept;

  array<Light*, MAX_DYNAMIC_LIGHTS> m_RegisteredLights;  // Array of pointers to registered lights

  GlobalLightingData m_GlobalLightingData;  // The global lighting information
};
