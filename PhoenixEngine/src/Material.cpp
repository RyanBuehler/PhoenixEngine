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
  m_Ambient(0.f),
  m_Diffuse(0.f),
  m_Specular(0.f),
  m_SpecularExp(0.f)
{
  setByType(Type::BASIC);
}

Material::Material(Type type) noexcept :
  m_Emissive(0.f),
  m_Ambient(0.f),
  m_Diffuse(0.f),
  m_Specular(0.f),
  m_SpecularExp(0.f)
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
  m_Ambient = material.m_Ambient;
  m_Diffuse = material.m_Diffuse;
  m_Specular = material.m_Specular;
  m_SpecularExp = material.m_SpecularExp;
  return *this;
}

void Material::SetEmissive(const vec3& value) noexcept
{
  m_Emissive = clamp(value, 0.f, 1.f);
}

void Material::SetAmbient(float value) noexcept
{
  m_Ambient = clamp(value, 0.f, 1.f);
}

void Material::SetDiffuse(float value) noexcept
{
  m_Diffuse = clamp(value, 0.f, 1.f);
}

void Material::SetSpecular(float value) noexcept
{
  m_Specular = clamp(value, 0.f, 1.f);
}

void Material::SetSpecularExp(float value) noexcept
{
  m_SpecularExp = clamp(value, 0.f, SPECEXPMAX);
}

const vec3& Material::GetEmissive() const noexcept
{
  return m_Emissive;
}

float Material::GetAmbient() const noexcept
{
  return m_Ambient;
}

float Material::GetDiffuse() const noexcept
{
  return m_Diffuse;
}

float Material::GetSpecular() const noexcept
{
  return m_Specular;
}

float Material::GetSpecularExp() const noexcept
{
  return m_SpecularExp;
}

void Material::setByType(Type type) noexcept
{
  //TODO: Eventually this could be loaded by material files
  switch (type)
  {
  case Material::Type::BASIC:
    m_Emissive = { 0.f, 0.f, 0.f };
    m_Ambient = 0.2f;
    m_Diffuse = 0.6f;
    m_Specular = 0.3f;
    m_SpecularExp = 10.f;
    break;
  case Material::Type::COUNT:
  default:
    break;
  }
}
