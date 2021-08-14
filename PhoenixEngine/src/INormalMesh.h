#pragma once
#include "ITextureMesh.h"

namespace Mesh
{
  /// <summary>
  /// Vertex Normal
  /// </summary>
  struct Normal
  {
    Normal(float x = 0.f, float y = 0.f, float z = 0.f) : X(x), Y(y), Z(z) {}
    Normal(Normal&) = default;
    Normal(const Normal&) = default;
    Normal(Normal&&) = delete;
    Normal& operator=(Normal&&) = delete;

    float X, Y, Z;
  };

  class INormalMesh : public Mesh::ITextureMesh
  {
  public:
    INormalMesh(Graphics::DataUsage dataUsage = Graphics::DataUsage::STATIC) noexcept :
      ITextureMesh(dataUsage)
    {};

    virtual const Normal* GetNormalArray() const = 0;

  private:
  };
}