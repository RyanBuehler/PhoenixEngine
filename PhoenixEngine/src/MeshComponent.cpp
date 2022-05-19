//------------------------------------------------------------------------------
// File:    MeshComponent.cpp
// Author:  Ryan Buehler
// Created: May 04, 2022
// Desc:    Renderable Mesh Component for game objects
//------------------------------------------------------------------------------
#include "pch.h"
#include "MeshComponent.h"

MeshComponent::MeshComponent(GameObject& parent) noexcept :
  Component(parent)
{
}

void MeshComponent::SetMeshFileName(const string& fileName) noexcept
{
  m_MeshFileName = fileName;
  m_MeshID = Error::INVALID_INDEX;
}