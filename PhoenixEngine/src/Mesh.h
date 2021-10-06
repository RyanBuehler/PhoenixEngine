//------------------------------------------------------------------------------
// File:    Mesh.h
// Author:  Ryan Buehler
// Created: 09/24/21
// Desc:    The class to handle the mesh data for rendering
//------------------------------------------------------------------------------
#pragma once
#include "GraphicsCommon.h"

  class Mesh
  {
#pragma region Features
  public:

    struct Edge
    {
      Edge(
        unsigned index1 = numeric_limits<unsigned>::max(),
        unsigned index2 = numeric_limits<unsigned>::max()
      ) : Index1(index1), Index2(index2) {}

      unsigned Index1, Index2;
    };

    struct Triangle
    {
      Triangle(
        unsigned index1 = numeric_limits<unsigned>::max(),
        unsigned index2 = numeric_limits<unsigned>::max(),
        unsigned index3 = numeric_limits<unsigned>::max()
      ) : Index1(index1), Index2(index2), Index3(index3) {}

      unsigned Index1, Index2, Index3;
    };

#pragma endregion

  public:
    Mesh(const vec3& origin = vec3(0.f, 0.f, 0.f),
      bool isStatic = true) noexcept;
    virtual ~Mesh() = default;

    //Mesh(const Mesh&) = delete;
    //Mesh& operator=(const Mesh&) = delete;
    //Mesh(Mesh&&) = delete;
    //Mesh& operator=(Mesh&&) = delete;

    unsigned GetVertexCount() const noexcept;
    unsigned GetTriangleCount() const noexcept;
    unsigned GetNormalCount() const noexcept;

    void AddVertex(vec3 vertex) noexcept;
    void AddVertex(float x, float y, float z) noexcept;

    void AddNormal(vec3 normal) noexcept;
    void AddNormal(float x, float y, float z) noexcept;

    void AddTriangle(unsigned index1, unsigned index2, unsigned index3) noexcept;

    inline void SetOrigin(const vec3& origin) noexcept { m_Origin = origin; }
    inline const vec3& GetOrigin() const noexcept { return m_Origin; }

    inline void SetIsStatic(bool isStatic) noexcept { m_bIsStatic = isStatic; }
    inline bool IsStatic() const noexcept { return m_bIsStatic; }

    vec3 CalculateBoundingBoxSize() noexcept;
    float CalculateWidestPoint() noexcept;
    void CalculateNormals(bool flipNormals) noexcept;
    void ScaleToUnitSize() noexcept;

  private:
    void CalculateSurfaceNormals() noexcept;
    void CalculateVertexNormals() noexcept;

    friend class MeshManager;

    vec3 m_Origin;
    bool m_bIsStatic;

    float m_NormalLength;

    vector<vec3> m_PositionArray;
    vector<vec3> m_VertexNormalArray;
    vector<vec3> m_SurfaceNormalArray;
    vector<Mesh::Triangle> m_TriangleArray;

    bool m_bIsDirty;
  };