//------------------------------------------------------------------------------
// File:    Light.cpp
// Author:  Ryan Buehler
// Created: Nov 5, 2021
// Desc:    Basic Light class, for an in game object that generates light
//------------------------------------------------------------------------------
#include "pch.h"
#include "Light.h"

Light::Light() noexcept :
  m_Transform(),
  m_Data()
{
}

const Light::Data& Light::GetData() noexcept
{
  m_Data.Position = vec4(m_Transform.GetPosition(), 1.f);
  return m_Data;
}

Transform& Light::GetTransform() noexcept
{
  return m_Transform;
}