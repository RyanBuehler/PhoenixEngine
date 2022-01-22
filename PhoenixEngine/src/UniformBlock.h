#pragma once
//------------------------------------------------------------------------------
// File:    UniformBlock.h
// Author:  Ryan Buehler
// Created: January 20, 2022
// Desc:    This class handles the setup and passing of Uniform Blocks of data
//          to the GPU.
//------------------------------------------------------------------------------
#include "GraphicsCommon.h"

class UniformBlock
{
public:
  UniformBlock() noexcept;
  ~UniformBlock() = default;
  UniformBlock(const UniformBlock&) = delete;
  UniformBlock& operator=(const UniformBlock&) = delete;
  UniformBlock(UniformBlock&&) = delete;
  UniformBlock& operator=(UniformBlock&&) = delete;

  //TODO: Go through all parameters and make them Pascal case
  void SetBlockName(const std::string& BlockName) noexcept;
  void SetStructName(const std::string& StructName) noexcept;
  void SetDataSize(size_t DataSize) noexcept;

  void AddDataMember(const std::string& DataMemberName) noexcept;

  void Create(GLuint ProgramID) noexcept;

  void SetDataPointer(void* DataPtr);

  void SendData() noexcept;

private:
  GLuint m_UBOIndex;  // Index of the UBO on the GPU

  std::string m_BlockName;  // Must match that of the shader's block
  std::string m_StructName; // Must match that of the shader's struct

  size_t m_DataSize; // Size of the data. Must match UBO size

  std::vector<std::string> m_DataMemberNames;

  //TODO: Do we even need to keep track?
  std::vector<GLuint> m_DataIndices;
  std::vector<GLint> m_DataOffsets;

  bool m_Created;

  void* m_DataPtr;
};
