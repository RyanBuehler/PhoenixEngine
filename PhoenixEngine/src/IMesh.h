#pragma once
#include "GraphicsCommon.h"
#include "IBindable.h"

namespace Mesh
{
#pragma region Vertex Attributes

  /// <summary>
  /// Vertex Position in 3D Space
  /// </summary>
  struct Position
  {
    Position(float x = 0.f, float y = 0.f, float z = 0.f) : X(x), Y(y), Z(z) {}
    Position(const Position&) = default;
    Position& operator=(const Position& other) = default;
    Position(Position&&) = delete;
    Position& operator=(Position&&) = delete;

    float X, Y, Z;
  };

#pragma endregion

#pragma region Features

  struct Edge
  {
    Edge(
      unsigned index1 = numeric_limits<unsigned>::max(),
      unsigned index2 = numeric_limits<unsigned>::max()
    ) : Index1(index1), Index2(index2) {}

    unsigned Index1, Index2;
  };

  struct Face
  {
    Face(
      unsigned index1 = numeric_limits<unsigned>::max(),
      unsigned index2 = numeric_limits<unsigned>::max(),
      unsigned index3 = numeric_limits<unsigned>::max()
    ) : Index1(index1), Index2(index2), Index3(index3) {}

    unsigned Index1, Index2, Index3;
  };

#pragma endregion

  class IMesh : public virtual IBindable
  {
  public:
    IMesh(Graphics::DataUsage dataUsage = Graphics::DataUsage::STATIC, const Position& origin = Position(0.f, 0.f, 0.f)) noexcept :
      m_Usage(dataUsage), m_Origin(origin) {};
    virtual ~IMesh() = default;

    IMesh(const IMesh&) = delete;
    IMesh& operator=(const IMesh&) = delete;
    IMesh(IMesh&&) = delete;
    IMesh& operator=(IMesh&&) = delete;

    virtual unsigned GetVertexCount() const noexcept = 0;
    virtual unsigned GetEdgeCount() const noexcept = 0;
    virtual unsigned GetFaceCount() const noexcept = 0;

    //TODO: Do I need these?
    virtual const Mesh::Position* GetPositionArray() const noexcept = 0;
    virtual const Mesh::Edge* GetEdgeArray() const noexcept = 0;
    virtual const Mesh::Face* GetFaceArray() const noexcept = 0;

    inline void SetOrigin(const Mesh::Position& origin) noexcept { m_Origin = origin; }
    inline const Mesh::Position& GetOrigin() const noexcept { return m_Origin; }

    inline void SetDataUsage(Graphics::DataUsage dataUsage) noexcept { m_Usage = dataUsage; }
    inline Graphics::DataUsage GetDataUsage() const noexcept { return m_Usage; }

  protected:
    Graphics::DataUsage m_Usage;
    Position m_Origin;
  };
}