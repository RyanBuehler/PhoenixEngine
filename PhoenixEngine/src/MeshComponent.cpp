//------------------------------------------------------------------------------
// File:    MeshComponent.cpp
// Author:  Ryan Buehler
// Created: May 04, 2022
// Desc:    Renderable Mesh Component for game objects
//------------------------------------------------------------------------------
#include "pch.h"
#include "MeshComponent.h"

MeshComponent::MeshComponent() noexcept :
  m_MeshID(Error::INVALID_INDEX)
{
}

void MeshComponent::SetMeshFileName(const string& FileName) noexcept
{
  m_MeshFileName = FileName;
  m_MeshID = Error::INVALID_INDEX;
}