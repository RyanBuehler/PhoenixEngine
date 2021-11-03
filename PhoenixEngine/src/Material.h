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
    BASIC,
    COUNT
  };

public:
  const float SPECEXPMAX = 20.f;

  Material() noexcept;
  Material(Type type) noexcept;
  ~Material() = default;
  Material(const Material& material);
  Material& operator=(const Material& material);
  Material(Material&&) = delete;
  Material& operator=(Material&&) = delete;

  void SetEmissive(const vec3& value) noexcept;

  void SetAmbient(float value) noexcept;
  void SetDiffuse(float value) noexcept;
  void SetSpecular(float value) noexcept;
  void SetSpecularExp(float value) noexcept;

  const vec3& GetEmissive() const noexcept;

  float GetAmbient() const noexcept;
  float GetDiffuse() const noexcept;
  float GetSpecular() const noexcept;
  float GetSpecularExp() const noexcept;

private:
  vec3 m_Emissive;

  float m_Ambient;
  float m_Diffuse;
  float m_Specular;

  float m_SpecularExp; //TODO:

  void setByType(Type type) noexcept;
};
