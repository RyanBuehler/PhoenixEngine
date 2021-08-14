#pragma once
#include "IMesh.h"

namespace Mesh
{
  /// <summary>
  /// Texture Coordinate
  /// </summary>
  struct TexCoord
  {
    TexCoord(float u = 0.f, float v = 0.f) : U(u), V(v) {}
    TexCoord(TexCoord&) = default;
    TexCoord(const TexCoord&) = default;
    TexCoord(TexCoord&&) = delete;
    TexCoord& operator=(TexCoord&&) = delete;

    float U, V;
  };

  class ITextureMesh : public IMesh
  {
  public:
    ITextureMesh(Graphics::DataUsage dataUsage = Graphics::DataUsage::STATIC) noexcept :
      IMesh(dataUsage)
    {};

    virtual const TexCoord* GetTexCoordArray() const = 0;

  private:
  };
}