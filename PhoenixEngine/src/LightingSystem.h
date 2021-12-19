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
  /// Sets the light attenuation for the scene
  /// </summary>
  /// <param name="constant">The constant factor in the attenuation</param>
  /// <param name="linear">The linear coefficient in the attenuation</param>
  /// <param name="quadratic">The quadratic coefficient in the attenuation</param>
  void SetLightAttenuation(float constant, float linear, float quadratic) noexcept;

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

  float GetLightAttenuationConstant() const noexcept;
  float GetLightAttenuationLinear() const noexcept;
  float GetLightAttenuationQuadratic() const noexcept;

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
