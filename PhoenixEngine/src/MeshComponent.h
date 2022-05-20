//------------------------------------------------------------------------------
// File:    MeshComponent.h
// Author:  Ryan Buehler
// Created: May 04, 2022
// Desc:    Renderable Mesh Component for game objects
//------------------------------------------------------------------------------
#pragma once
#include "Component.h"
#include "Material.h"

class MeshComponent final : public Component
{
  public:
  MeshComponent() noexcept;
  ~MeshComponent() override = default;
  MeshComponent(const MeshComponent&) = delete;
  MeshComponent& operator=(const MeshComponent&) = delete;
  MeshComponent(MeshComponent&&) = delete;
  MeshComponent& operator=(MeshComponent&&) = delete;

  [[nodiscard]] Type GetType() const noexcept override { return Type::MESH; }

  void SetMeshFileName(const string& FileName) noexcept;
  [[nodiscard]] const string& GetMeshFileName() const noexcept { return m_MeshFileName; }

  void SetMaterial(const Material& Material) noexcept { m_Material = Material; }
  const Material& GetMaterial() noexcept { return m_Material; }
  [[nodiscard]] unsigned GetMeshID() const noexcept { return m_MeshID; }
  void SetMeshID(const unsigned MeshID) noexcept { m_MeshID = MeshID; }

  private:
  unsigned m_MeshID;
  string m_MeshFileName;
  Material m_Material;
};
