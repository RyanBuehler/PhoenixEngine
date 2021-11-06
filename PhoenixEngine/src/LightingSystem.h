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
  const static unsigned LIGHT_ERROR = numeric_limits<unsigned>::max();
  const static unsigned MAX_DYNAMIC_LIGHTS = 16;

  struct GlobalLightingData
  {
    GlobalLightingData() noexcept :
      AmbientIntensity({ 0.1f, 0.1f, 0.1f }),
      FogIntensity({ 0.1f, 0.1f, 0.1f }),
      FogNear(1.f),
      FogFar(20.f),
      Attenuation()
    {
      Attenuation[0] = 0.1f;
      Attenuation[1] = 0.1f;
      Attenuation[2] = 0.1f;
    }
    vec3 AmbientIntensity;        // Global Ambient intensity
    vec3 FogIntensity;            // Global fog intensity
    float FogNear;                // Fog near factor
    float FogFar;                 // Fog far factor
    array<float,3> Attenuation;   // Light attenuation
  };

public:
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
  /// <param name="light"></param>
  /// <returns></returns>
  bool RegisterLight(Light* lightPtr) noexcept;

  /// <summary>
  /// Deregisters a light by pointer
  /// </summary>
  /// <param name="light">A pointer to the registered light</param>
  /// <returns>T/F of Success or Failure</returns>
  bool DerigesterLight(Light* lightPtr) noexcept;

  /// <summary>
  /// Checks if a free light slot exists
  /// </summary>
  /// <returns>T/F if the light slot exists</returns>
  bool FreeLightSlotExists() const noexcept;

  /// <summary>
  /// Gets the Global Lighting Data for the scene
  /// </summary>
  /// <returns>[Const] Reference to the Global Lighting Data</returns>
  const GlobalLightingData& GetGlobalLightingData() const noexcept;

  /// <summary>
  /// Sets the Global Ambient Intensity for the scene
  /// </summary>
  /// <param name="globalAmbientIntensity">The light intensity for the global ambience</param>
  void SetGlobalAmbientIntensity(const vec3& globalAmbientIntensity) noexcept;

  /// <summary>
  /// Sets the Global Fog Intensity
  /// </summary>
  /// <param name="globalFogIntensity">The light intensity for the fog</param>
  void SetGlobalFogIntensity(const vec3& globalFogIntensity) noexcept;

  /// <summary>
  /// Sets the near value for fog calculations
  /// </summary>
  /// <param name="fogNear">The near distance for fog calculations</param>
  void SetFogNearDistance(float fogNear) noexcept;

  /// <summary>
  /// Sets the far value for fog calculations
  /// </summary>
  /// <param name="fogFar">The far distance for fog calculations</param>
  void SetFogFarDistance(float fogFar) noexcept;

  /// <summary>
  /// Sets the global light attenuation (falloff)
  /// </summary>
  /// <param name="c0">The constant parameter</param>
  /// <param name="c1">The first degree parameter</param>
  /// <param name="c2">The second degree parameter</param>
  void SetGlobalLightAttenuation(float c0, float c1, float c2) noexcept;

  /// <summary>
  /// Sets the global light attenuation (falloff)
  /// </summary>
  /// <param name="attenuation">3 Float Array of constant, first and second degree values</param>
  void SetGlobalLightAttenuation(const array<float, 3>& attenuation);

  /// <summary>
  /// Gets the Global Ambient Intensity
  /// </summary>
  /// <returns>[const] The global ambient intensity value</returns>
  const vec3& GetGlobalAmbientIntensity() const noexcept;

  /// <summary>
  /// Gets the Global Fog Intensity
  /// </summary>
  /// <returns>[const] The global fog intensity value</returns>
  const vec3& GetGlobalFogIntensity() const noexcept;

  /// <summary>
  /// Gets the fog near distance
  /// </summary>
  /// <returns>The fog near distance value</returns>
  float GetFogNearDistance() const noexcept;

  /// <summary>
  /// Gets the fog far distance
  /// </summary>
  /// <returns>The fog far distance value</returns>
  float GetFogFarDistance() const noexcept;

  /// <summary>
  /// Gets the Global Light Attenuation
  /// </summary>
  /// <returns>The fog near distance value</returns>
  const array<float,3>& GetGlobalLightAttenuation() const noexcept;

private:
  /// <summary>
  /// Helper function to get the number of active lights
  /// </summary>
  /// <returns>The active light count</returns>
  unsigned getActiveLightCount() const noexcept;

  /// <summary>
  /// Helper function that gets the number of lights free to register
  /// </summary>
  /// <returns>The count of light slots still open</returns>
  unsigned getFreeLightCount() const noexcept;

private:
  array<Light*, MAX_DYNAMIC_LIGHTS> m_RegisteredLights;  // Array of pointers to registered lights

  bool m_LightingIsDirty;                   // Dirty if the lighting system has a new light registered or removed
  unsigned m_ActiveLightLimit;              // Limit of the number of active lights in the scene

  GlobalLightingData m_GlobalLightingData;  // The global lighting information
};
