//------------------------------------------------------------------------------
// File:    Material.h
// Author:  Ryan Buehler
// Created: 11/01/21
// Desc:    Material Class for Phong lighting
//------------------------------------------------------------------------------
#pragma once
#include "GraphicsCommon.h"

class Material
{
public:
  enum class Type
  {
    CUSTOM,
    TEXTURE,
    //REFLECTREFRACT,
    //REFRACTIVE,
    BASIC,
    LIGHT,
    GLOBAL,
    COUNT
  };

public:
  const float SPEC_EXP_MAX = 20.f;  // An upper limit to clamp specular highlight exponent

  /// <summary>
  /// Material Default Constructor
  /// </summary>
  Material() noexcept;
  
  /// <summary>
  /// Material Constructor by enum type
  /// </summary>
  /// <param name="type">The predefined material type</param>
  Material(Type type) noexcept;

  // Rule of 5
  ~Material() = default;
  Material(const Material& material);
  Material& operator=(const Material& material);
  Material(Material&&) = delete;
  Material& operator=(Material&&) = delete;

  /// <summary>
  /// Sets the emissive value of the material (light emitted)
  /// </summary>
  /// <param name="value">The emissive RGB value</param>
  void SetEmissive(const vec3& value) noexcept;

  /// <summary>
  /// Sets the ambient value of the material (ambient light absorbed)
  /// </summary>
  /// <param name="value">The ambient multiplier</param>
  void SetAmbient(float value) noexcept;

  /// <summary>
  /// Sets the diffuse value of the material (direct light absorbed)
  /// </summary>
  /// <param name="value">The diffuse multiplier</param>
  void SetDiffuse(float value) noexcept;

  /// <summary>
  /// Sets the specular value of the material (light reflected)
  /// </summary>
  /// <param name="value">The specular multiplier</param>
  void SetSpecular(float value) noexcept;

  /// <summary>
  /// Sets the specular exponent of the material (shininess)
  /// </summary>
  /// <param name="value">The specular exponent</param>
  /// <returns></returns>
  void SetSpecularExp(float value) noexcept;

  /// <summary>
  /// Gets the Emissive RGB values
  /// </summary>
  /// <returns>[Const] The RGB emissive value</returns>
  const vec3& GetEmissive() const noexcept;

  /// <summary>
  /// Gets the ambient value of the material (ambient light absorbed)
  /// </summary>
  /// <returns>The ambient light factor</returns>
  float GetAmbient() const noexcept;

  /// <summary>
  /// Gets the diffuse value of the material (direct light absorbed)
  /// </summary>
  /// <returns>The diffuse light factor</returns>
  float GetDiffuse() const noexcept;

  /// <summary>
  /// Gets the specular value of the material (light reflected)
  /// </summary>
  /// <returns>The specular light factor</returns>
  float GetSpecular() const noexcept;

  /// <summary>
  /// Sets the specular exponent of the material (shininess)
  /// </summary>
  /// <returns>The specular exponent factor</returns>
  float GetSpecularExp() const noexcept;

  /// <summary>
  /// Gets the material type if it exists
  /// </summary>
  /// <returns>The type of the material</returns>
  Type GetType() const noexcept;

private:
  vec3 m_Emissive;            // How much light is emitted

  float m_AmbientFactor;      // How much ambient light is absorbed
  float m_DiffuseFactor;      // How much light is absorbed
  float m_SpecularFactor;     // How much light is reflected

  float m_SpecularExp;        // The shininness of the object

  Type m_Type;                // Type if it exists


  /// <summary>
  /// Helper function for predefined defaults of Material types
  /// </summary>
  /// <param name="type">The predefined material type</param>
  void setByType(Type type) noexcept;

#ifdef _IMGUI
  friend class ImGuiManager;
#endif
};
