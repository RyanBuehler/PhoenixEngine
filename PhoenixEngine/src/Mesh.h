//------------------------------------------------------------------------------
// File:    Mesh.h
// Author:  Ryan Buehler
// Created: 09/24/21
// Desc:    The class to handle the mesh data for rendering
//------------------------------------------------------------------------------
#pragma once
#include "GraphicsCommon.h" // GLEW and Common Graphics types

  class Mesh
  {

#pragma region Features
  public:
    /// <summary>
    /// Per Vertex Data to be sent to the GPU
    /// </summary>
    struct VertexData
    {
      vec3 Position;  // Vertex Position
      vec3 Normal;    // Vertex Normal
      vec2 Texcoord;  // Texture UV Coordinates
    };

    /// <summary>
    /// Index-based data structure for mesh edges
    /// </summary>
    struct Edge
    {
      Edge(
        const unsigned Index1 = Error::INVALID_INDEX,
        const unsigned Index2 = Error::INVALID_INDEX
      ) : Index1(Index1), Index2(Index2) {}

      unsigned Index1, Index2; // Indices of the vertices composing the edge
    };

    /// <summary>
    /// Index-based collection of triangle (face) vertices
    /// </summary>
    struct Triangle
    {
      Triangle(
        const unsigned Index1 = Error::INVALID_INDEX,
        const unsigned Index2 = Error::INVALID_INDEX,
        const unsigned Index3 = Error::INVALID_INDEX
      ) : Index1(Index1), Index2(Index2), Index3(Index3) {}

      unsigned Index1, Index2, Index3; // Indices of the vertices composing the triangle
    };

#pragma endregion

    struct BoundingBox
    {
      float XMin, YMin, ZMin, XMax, YMax, ZMax;
    };

    /// <summary>
    /// Default constructor
    /// </summary>
    /// <param name="Origin">The object "origin" or pivot point</param>
    /// <param name="bIsStatic">[T/F] The mesh is static, not dynamic</param>
    Mesh(const vec3& Origin = vec3(0.f, 0.f, 0.f), bool bIsStatic = true) noexcept;

    ~Mesh() = default;
    Mesh(const Mesh&) = default;
    Mesh& operator=(const Mesh&) = default;
    Mesh(Mesh&&) = default;
    Mesh& operator=(Mesh&&) = default;
    

    /// <summary>
    /// Gets the number of vertices of the mesh
    /// </summary>
    /// <returns>The number of vertices in the mesh</returns>
    [[nodiscard]] unsigned GetVertexCount() const noexcept;

    /// <summary>
    /// Gets the number of triangles composing the mesh
    /// </summary>
    /// <returns>The number of triangles composing the mesh</returns>
    [[nodiscard]] unsigned GetTriangleCount() const noexcept;

    /// <summary>
    /// Gets the number of vertex normals, although this should match vertices
    /// </summary>
    /// <returns>The number of vertex normals in the mesh</returns>
    [[nodiscard]] unsigned GetVertexNormalCount() const noexcept;

    /// <summary>
    /// Gets the number of UV coordinates, although this should match vertices
    /// </summary>
    /// <returns>The number of UV coordinates in the mesh</returns>
    [[nodiscard]] unsigned GetTexcoordCount() const noexcept;

    /// <summary>
    /// Adds a single vertex to the mesh
    /// </summary>
    /// <param name="Vertex">[Const Ref] Location of the vertex in object space</param>
    void AddVertex(const vec3& Vertex) noexcept;

    /// <summary>
    /// Manually adds a single vertex to the mesh
    /// </summary>
    /// <param name="X">X coordinate of the new vertex in object space</param>
    /// <param name="Y">Y coordinate of the new vertex in object space</param>
    /// <param name="Z">Z coordinate of the new vertex in object space</param>
    /// <returns></returns>
    void AddVertex(float X, float Y, float Z) noexcept;

    /// <summary>
    /// Manually adds a single vertex normal to the mesh
    /// </summary>
    /// <param name="Normal">[Const Ref] Vector of the vertex normal</param>
    void AddVertexNormal(const vec3& Normal) noexcept;

    /// <summary>
    /// Manually adds a single vertex normal to the mesh
    /// </summary>
    /// <param name="X">X value of the new vertex normal</param>
    /// <param name="Y">Y value of the new vertex normal</param>
    /// <param name="Z">Z value of the new vertex normal</param>
    void AddVertexNormal(float X, float Y, float Z) noexcept;

    void AddTexcoord(const vec2& Texcoord) noexcept;

    void AddTexcoord(float U, float V);

    /// <summary>
    /// Manually adds a single triangle (via vertex indices) to the mesh
    /// </summary>
    /// <param name="Index1">The index of the first vertex</param>
    /// <param name="Index2">The index of the second vertex</param>
    /// <param name="Index3">The index of the third vertex</param>
    /// <returns></returns>
    void AddTriangle(unsigned Index1, unsigned Index2, unsigned Index3) noexcept;

    /// <summary>
    /// Sets the origin of the mesh to a new location in object space
    /// </summary>
    /// <param name="Origin">The new origin in object space</param>
    void SetOrigin(const vec3& Origin) noexcept { m_Origin = Origin; }
    
    /// <summary>
    /// Gets the origin of the mesh
    /// </summary>
    /// <returns>[Const Ref] Location of the origin in object space</returns>
    [[nodiscard]] const vec3& GetOrigin() const noexcept { return m_Origin; }

    /// <summary>
    /// Sets the mesh to be static or dynamic (not static)
    /// </summary>
    /// <param name="bIsStatic">[T/F] The mesh will be static</param>
    void SetMeshIsStatic(const bool bIsStatic) noexcept { m_MeshIsStatic = bIsStatic; }

    /// <summary>
    /// Sets that the normals have been calculated
    /// </summary>
    /// <param name="bAreNormalsCalculated">[T/F] The normals have been calculated</param>
    void SetNormalsAreCalculated(const bool bAreNormalsCalculated) noexcept { m_MeshIsStatic = bAreNormalsCalculated; }

    /// <summary>
    /// Gets whether or not the mesh is set to be static
    /// </summary>
    /// <returns>[T/F] The mesh is set to static</returns>
    [[nodiscard]] bool MeshIsStatic() const noexcept { return m_MeshIsStatic; }

    /// <summary>
    /// Gets whether or not the normals are calculated from either import or calculation
    /// </summary>
    /// <returns>[T/F] The mesh has normals calculated</returns>
    [[nodiscard]] bool NormalsAreCalculated() const noexcept { return m_NormalsAreCalculated; }

    /// <summary>
    /// Gets the Vertex Normal Array
    /// </summary>
    /// <returns>[Const Ref] The vector of vertex normals</returns>
    [[nodiscard]] const vector<vec3>& GetVertexNormalArray() const noexcept;

    /// <summary>
    /// Gets the Surface Normal Array
    /// </summary>
    /// <returns>[Const Ref] The vector of surface normals</returns>
    [[nodiscard]] const vector<vec3>& GetSurfaceNormalArray() const noexcept;

    /// <summary>
    /// Gets the Surface Normal Position Array (position in object space to draw normals)
    /// </summary>
    /// <returns>[Const Ref] The vector of surface normal positions</returns>
    [[nodiscard]] const vector<vec3>& GetSurfaceNormalPositionArray() const noexcept;

    /// <summary>
    /// Gets the UV Coordinate Array
    /// </summary>
    /// <returns>[Const Ref] The vector of UV coordinates</returns>
    [[nodiscard]] const vector<vec2>& GetTexcoordArray() const noexcept;

    /// <summary>
    /// Calculates the bounding box around the mesh in object space (min->max in x,y,z)
    /// </summary>
    /// <returns>A structure of min/max in x,y,z</returns>
    [[nodiscard]] BoundingBox CalculateBoundingBox() const noexcept;

    /// <summary>
    /// Calculates the bounding box size around the mesh in object space (min->max in x,y,z)
    /// </summary>
    /// <returns>A 3 float vector of the length between min and max in each dimension</returns>
    [[nodiscard]] vec3 CalculateBoundingBoxSize() const noexcept;

    /// <summary>
    /// Calculates the greatest width of the three dimensions
    /// </summary>
    /// <returns>The width at the greatest of the 3 dimensions</returns>
    [[nodiscard]] float CalculateWidestPoint() const noexcept;

    /// <summary>
    /// Calculates the normals based on current vertices
    /// </summary>
    /// <param name="bShouldFlipNormals">[T/F] The normals should be inverted</param>
    void CalculateNormals(bool bShouldFlipNormals = false) noexcept;

    /// <summary>
    /// Scale the mesh to unit size (fit into a 1x1x1 cube)
    /// </summary>
    void ScaleToUnitSize() noexcept;

    /// <summary>
    /// Finds the centroid of the mesh via vertex averages
    /// </summary>
    /// <returns>The location of the centroid in object space</returns>
    [[nodiscard]] vec3 FindCentroid() const noexcept;

    /// <summary>
    /// Sets the origin of the mesh to the center of mass
    /// </summary>
    void ResetOriginToCentroid() noexcept;

    /// <summary>
    /// Generates UV coordinates based on mathematical projections
    /// </summary>
    /// <param name="Generation">The UV generation technique</param>
    /// <returns></returns>
    void GenerateTexcoords(UV::Generation Generation) noexcept;

    /// <summary>
    /// Assembles all mesh data into VertexData to be sent to the GPU
    /// </summary>
    void AssembleVertexData() noexcept;

  private:
    /// <summary>
    /// Helper function to calculate surface normals
    /// </summary>
    /// <param name="bShouldFlipNormals">[T/F] The normals should be inverted</param>
    void calculateSurfaceNormals(bool bShouldFlipNormals = false) noexcept;

    /// <summary>
    /// Helper function to calculate vertex normals
    /// </summary>
    void calculateVertexNormals() noexcept;

    /// <summary>
    /// Helper function to calculate UV coordinates with a planar projection
    /// </summary>
    void calculatePlanarUVs() noexcept;

    /// <summary>
    /// Helper function to calculate UV coordinates with a spherical projection
    /// </summary>
    void calculateSphereUVs() noexcept;

    /// <summary>
    /// Helper function to calculate UV coordinates with a cylindrical projection
    /// </summary>
    /// <returns></returns>
    void calculateCylinderUVs() noexcept;
    
    /// <summary>
    /// [Not Implemented] Helper function to calculate UV coordinates with a cube map
    /// </summary>
    void calculateCubeMapUVs() const noexcept;

    friend class AssetLoader;
    friend class MeshManager; // Allows the Mesh Manager class exclusive access

    vec3 m_Origin;        // The mesh's origin point (pivot point)
    bool m_MeshIsStatic;  // [T/F] The mesh is static (not dynamic)

    float m_NormalLength; // The length of the debug draw lines to showcase normals

    vector<vec3> m_PositionArray;               // Vertex positions
    vector<vec3> m_VertexNormalArray;           // Vertex normals
    vector<vec3> m_SurfaceNormalArray;          // Surface normals
    vector<vec3> m_SurfaceNormalPositionArray;  // Positions of surface normals
    vector<Triangle> m_TriangleArray;           // Indices of vertices to create triangles
    vector<vec2> m_TexcoordArray;               // UV coordinates
    
    vector<VertexData> m_VertexData;            // GPU data for rendering

    bool m_MeshIsDirty;                         // [T/F] The mesh has changed fundamentally
    bool m_NormalsAreCalculated;                // [T/F] If the normals have been calculated (ie. imported, or calculated)
  };