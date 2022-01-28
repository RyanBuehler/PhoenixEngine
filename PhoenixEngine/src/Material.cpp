//------------------------------------------------------------------------------
// File:    Material.h
// Author:  Ryan Buehler
// Created: 11/01/21
// Desc:    Material Class for Phong lighting
//------------------------------------------------------------------------------
#include "pch.h"
#include "Material.h"

Material::Material() noexcept :
  m_Emissive(0.f),
  m_AmbientFactor(0.f),
  m_DiffuseFactor(0.f),
  m_SpecularFactor(0.f),
  m_SpecularExp(0.f),
  m_Type(Type::BASIC)
{
  setByType(m_Type);
}

Material::Material(Type type) noexcept :
  m_Emissive(0.f),
  m_AmbientFactor(0.f),
  m_DiffuseFactor(0.f),
  m_SpecularFactor(0.f),
  m_SpecularExp(0.f),
  m_Type(type)
{
  setByType(type);
}

Material::Material(const Material& material)
{
  *this = material;
}

Material& Material::operator=(const Material& material)
{
  m_Emissive = material.m_Emissive;
  m_AmbientFactor = material.m_AmbientFactor;
  m_DiffuseFactor = material.m_DiffuseFactor;
  m_SpecularFactor = material.m_SpecularFactor;
  m_SpecularExp = material.m_SpecularExp;
  m_Type = material.m_Type;
  return *this;
}

void Material::SetEmissive(const vec3& value) noexcept
{
  m_Emissive = clamp(value, 0.f, 1.f);
  m_Type = Type::CUSTOM;
}

void Material::SetAmbient(float value) noexcept
{
  m_AmbientFactor = clamp(value, 0.f, 1.f);
  m_Type = Type::CUSTOM;
}

void Material::SetDiffuse(float value) noexcept
{
  m_DiffuseFactor = clamp(value, 0.f, 1.f);
  m_Type = Type::CUSTOM;
}

void Material::SetSpecular(float value) noexcept
{
  m_SpecularFactor = clamp(value, 0.f, 1.f);
  m_Type = Type::CUSTOM;
}

void Material::SetSpecularExp(float value) noexcept
{
  m_SpecularExp = clamp(value, 0.f, SPEC_EXP_MAX);
  m_Type = Type::CUSTOM;
}

const vec3& Material::GetEmissive() const noexcept
{
  return m_Emissive;
}

float Material::GetAmbient() const noexcept
{
  return m_AmbientFactor;
}

float Material::GetDiffuse() const noexcept
{
  return m_DiffuseFactor;
}

float Material::GetSpecular() const noexcept
{
  return m_SpecularFactor;
}

float Material::GetSpecularExp() const noexcept
{
  return m_SpecularExp;
}

Material::Type Material::GetType() const noexcept
{
  return m_Type;
}

void Material::setByType(Type type) noexcept
{
  //TODO: Eventually this could be loaded by material files
  switch (type)
  {
  //case Material::Type::REFLECTREFRACT:
  //case Material::Type::REFRACTIVE:
  //  break;
  case Material::Type::BASIC:
    m_Emissive = { 0.f, 0.f, 0.f };
    m_AmbientFactor = 0.3f;
    m_DiffuseFactor = 0.9f;
    m_SpecularFactor = 1.f;
    m_SpecularExp = 300.f;
    m_Type = Type::BASIC;
    break;
  case Material::Type::LIGHT:
    m_Emissive = { 1.f, 1.f, 1.f };
    m_AmbientFactor = 0.0f;
    m_DiffuseFactor = 0.0f;
    m_SpecularFactor = 0.0f;
    m_SpecularExp = 0.01f;
    m_Type = Type::LIGHT;
    break;
  case Material::Type::GLOBAL:
    m_Type = Type::GLOBAL;
    break;
  case Material::Type::CUSTOM:
    m_Type = Type::CUSTOM;
    break;
  case Material::Type::TEXTURE:
    m_Emissive = { 0.f, 0.f, 0.f };
    m_AmbientFactor = 0.25f;
    m_DiffuseFactor = 1.0f;
    m_SpecularFactor = 1.f;
    m_SpecularExp = 300.f;
    m_Type = Type::TEXTURE;
    break;
  case Material::Type::COUNT:
  default:
    break;
  }
}
