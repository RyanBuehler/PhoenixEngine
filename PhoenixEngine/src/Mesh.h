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
        unsigned index1 = Error::INVALID_INDEX,
        unsigned index2 = Error::INVALID_INDEX
      ) : Index1(index1), Index2(index2) {}

      unsigned Index1, Index2; // Indices of the vertices comprosing the edge
    };

    /// <summary>
    /// Index-based collection of triangle (face) vertices
    /// </summary>
    struct Triangle
    {
      Triangle(
        unsigned index1 = Error::INVALID_INDEX,
        unsigned index2 = Error::INVALID_INDEX,
        unsigned index3 = Error::INVALID_INDEX
      ) : Index1(index1), Index2(index2), Index3(index3) {}

      unsigned Index1, Index2, Index3; // Indices of the vertices comprosing the triangle
    };

#pragma endregion

    struct BoundingBox
    {
      float xMin, yMin, zMin, xMax, yMax, zMax;
    };

  public:
    /// <summary>
    /// Default constructor
    /// </summary>
    /// <param name="origin">The object "origin" or pivot point</param>
    /// <param name="isStatic">[T/F] The mesh is static, not dynamic</param>
    Mesh(const vec3& origin = vec3(0.f, 0.f, 0.f), bool isStatic = true) noexcept;

    /// <summary>
    /// Default destructor
    /// </summary>
    ~Mesh() = default;

    /// <summary>
    /// Gets the number of vertices of the mesh
    /// </summary>
    /// <returns>The number of vertices in the mesh</returns>
    unsigned GetVertexCount() const noexcept;

    /// <summary>
    /// Gets the number of triangles composing the mesh
    /// </summary>
    /// <returns>The number of triangles composing the mesh</returns>
    unsigned GetTriangleCount() const noexcept;

    /// <summary>
    /// Gets the number of vertex normals, although this should match vertices
    /// </summary>
    /// <returns>The number of vertex normals in the mesh</returns>
    unsigned GetVertexNormalCount() const noexcept;

    /// <summary>
    /// Gets the number of UV coordinates, although this should match vertices
    /// </summary>
    /// <returns>The number of UV coordinates in the mesh</returns>
    unsigned GetTexcoordCount() const noexcept;

    /// <summary>
    /// Adds a single vertex to the mesh
    /// </summary>
    /// <param name="vertex">[Const Ref] Location of the vertex in object space</param>
    void AddVertex(const vec3& vertex) noexcept;

    /// <summary>
    /// Manually adds a single vertex to the mesh
    /// </summary>
    /// <param name="x">X coordinate of the new vertex in object space</param>
    /// <param name="y">Y coordinate of the new vertex in object space</param>
    /// <param name="z">Z coordinate of the new vertex in object space</param>
    /// <returns></returns>
    void AddVertex(float x, float y, float z) noexcept;

    /// <summary>
    /// Manually adds a single vertex normal to the mesh
    /// </summary>
    /// <param name="normal">[Const Ref] Vector of the vertex normal</param>
    void AddVertexNormal(const vec3& normal) noexcept;

    /// <summary>
    /// Manually adds a single vertex normal to the mesh
    /// </summary>
    /// <param name="x">X value of the new vertex normal</param>
    /// <param name="y">Y value of the new vertex normal</param>
    /// <param name="z">Z value of the new vertex normal</param>
    void AddVertexNormal(float x, float y, float z) noexcept;

    /// <summary>
    /// Manually adds a single triangle (via vertex indices) to the mesh
    /// </summary>
    /// <param name="index1">The index of the first vertex</param>
    /// <param name="index2">The index of the second vertex</param>
    /// <param name="index3">The index of the third vertex</param>
    /// <returns></returns>
    void AddTriangle(unsigned index1, unsigned index2, unsigned index3) noexcept;

    /// <summary>
    /// Sets the origin of the mesh to a new location in object space
    /// </summary>
    /// <param name="origin">The new origin in object space</param>
    inline void SetOrigin(const vec3& origin) noexcept { m_Origin = origin; }
    
    /// <summary>
    /// Gets the origin of the mesh
    /// </summary>
    /// <returns>[Const Ref] Location of the origin in object space</returns>
    inline const vec3& GetOrigin() const noexcept { return m_Origin; }

    /// <summary>
    /// Sets the mesh to be static or dynamic (not static)
    /// </summary>
    /// <param name="isStatic">[T/F] The mesh will be static</param>
    inline void SetMeshIsStatic(bool isStatic) noexcept { m_MeshIsStatic = isStatic; }

    /// <summary>
    /// Sets that the normals have been calculated
    /// </summary>
    /// <param name="isStatic">[T/F] The normals have been calculated</param>
    inline void SetNormalsAreCalculated(bool areNormalsCalculated) noexcept { m_MeshIsStatic = areNormalsCalculated; }

    /// <summary>
    /// Gets whether or not the mesh is set to be static
    /// </summary>
    /// <returns>[T/F] The mesh is set to static</returns>
    inline bool MeshIsStatic() const noexcept { return m_MeshIsStatic; }

    /// <summary>
    /// Gets whether or not the normals are calculated from either import or calculation
    /// </summary>
    /// <returns>[T/F] The mesh has normals calculated</returns>
    inline bool NormalsAreCalculated() const noexcept { return m_NormalsAreCalculated; }

    /// <summary>
    /// Gets the Vertex Normal Array
    /// </summary>
    /// <returns>[Const Ref] The vector of vertex normals</returns>
    const vector<vec3>& GetVertexNormalArray() const noexcept;

    /// <summary>
    /// Gets the Surface Normal Array
    /// </summary>
    /// <returns>[Const Ref] The vector of surface normals</returns>
    const vector<vec3>& GetSurfaceNormalArray() const noexcept;

    /// <summary>
    /// Gets the Surface Normal Position Array (position in object space to draw normals)
    /// </summary>
    /// <returns>[Const Ref] The vector of surface normal positions</returns>
    const vector<vec3>& GetSurfaceNormalPositionArray() const noexcept;

    /// <summary>
    /// Gets the UV Coordinate Array
    /// </summary>
    /// <returns>[Const Ref] The vector of UV coordinates</returns>
    const vector<vec2>& GetTexcoordArray() const noexcept;

    /// <summary>
    /// Calculates the bounding box around the mesh in object space (min->max in x,y,z)
    /// </summary>
    /// <returns>A structure of min/max in x,y,z</returns>
    BoundingBox CalculateBoundingBox() const noexcept;

    /// <summary>
    /// Calculates the bounding box size around the mesh in object space (min->max in x,y,z)
    /// </summary>
    /// <returns>A 3 float vector of the length between min and max in each dimension</returns>
    vec3 CalculateBoundingBoxSize() noexcept;

    /// <summary>
    /// Calculates the greatest width of the three dimensions
    /// </summary>
    /// <returns>The width at the greatest of the 3 dimensions</returns>
    float CalculateWidestPoint() noexcept;

    /// <summary>
    /// Calculates the normals based on current vertices
    /// </summary>
    /// <param name="flipNormals">[T/F] The normals should be inverted</param>
    void CalculateNormals(bool flipNormals = false) noexcept;

    /// <summary>
    /// Scale the mesh to unit size (fit into a 1x1x1 cube)
    /// </summary>
    void ScaleToUnitSize() noexcept;

    /// <summary>
    /// Finds the centroid of the mesh via vertex averages
    /// </summary>
    /// <returns>The location of the centroid in object space</returns>
    vec3 FindCentroid() const noexcept;

    /// <summary>
    /// Sets the origin of the mesh to the center of mass
    /// </summary>
    void ResetOriginToCentroid() noexcept;

    /// <summary>
    /// Generates UV coordinates based on mathematical projections
    /// </summary>
    /// <param name="generation">The UV generation technique</param>
    /// <returns></returns>
    void GenerateTexcoords(UV::Generation generation) noexcept;

    /// <summary>
    /// Assembles all mesh data into VertexData to be sent to the GPU
    /// </summary>
    void AssembleVertexData() noexcept;

  private:
    /// <summary>
    /// Helper function to calculate surface normals
    /// </summary>
    /// <param name="flipNormals">[T/F] The normals should be inverted</param>
    void calculateSurfaceNormals(bool flipNormals = false) noexcept;

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
    void calculateCubeMapUVs() noexcept;

  private:
    friend class MeshManager; // Allows the Mesh Manager class exclusive access

    vec3 m_Origin;        // The mesh's origin point (pivot point)
    bool m_MeshIsStatic;  // [T/F] The mesh is static (not dynamic)

    float m_NormalLength; // The length of the debug draw lines to showcase normals

    vector<vec3> m_PositionArray;               // Vertex positions
    vector<vec3> m_VertexNormalArray;           // Vertex normals
    vector<vec3> m_SurfaceNormalArray;          // Surface normals
    vector<vec3> m_SurfaceNormalPositionArray;  // Positions of surface normals
    vector<Mesh::Triangle> m_TriangleArray;     // Indices of vertices to create triangles
    vector<vec2> m_TexcoordArray;               // UV coordinates
    
    vector<VertexData> m_VertexData;            // GPU data for rendering

    bool m_MeshIsDirty;                         // [T/F] The mesh has changed fundamentally
    bool m_NormalsAreCalculated;                // [T/F] If the normals have been calculated (ie. imported, or calculated)
  };