#pragma once
#include "IBindable.h"
#include "GraphicsCommon.h"

class VertexArrayObject : public IBindable
{
public:
  struct VertexObject
  {
    float position[3];
    float normal[3];
    float texcoord[2];
  };

public:
  VertexArrayObject() noexcept;
  VertexArrayObject(Graphics::DataUsage dataUsage) noexcept;
  ~VertexArrayObject() noexcept;
  VertexArrayObject(const VertexArrayObject&) = delete;
  VertexArrayObject& operator=(const VertexArrayObject&) = delete;
  VertexArrayObject(VertexArrayObject&&) = delete;
  VertexArrayObject& operator=(VertexArrayObject&&) = delete;

  void Bind() noexcept override;

  /*inline void AddVertex(const Vertex& vertex) noexcept;*/
  void AddVertex(float x, float y, float z) noexcept;

  inline const float* GetVertexArray() const noexcept;

  //TODO Should this be in IBindable?
  inline void SetDataUsage(Graphics::DataUsage dataUsage) noexcept;
  inline Graphics::DataUsage GetDataUsage() const noexcept;

  void ClearArray() noexcept;

private:
  unsigned m_BufferID;
  vector<float> m_Vertices;

  bool m_IsDirty;

  Graphics::DataUsage m_DataUsage;
};

