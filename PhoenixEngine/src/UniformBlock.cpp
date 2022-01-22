//------------------------------------------------------------------------------
// File:    UniformBlock.cpp
// Author:  Ryan Buehler
// Created: January 20, 2022
// Desc:    This class handles the setup and passing of Uniform Blocks of data
//          to the GPU.
//------------------------------------------------------------------------------
#include "pch.h"
#include "UniformBlock.h"

UniformBlock::UniformBlock() noexcept :
  m_UBOIndex(Error::INVALID_INDEX),
  m_BlockName("Unnamed Block"),
  m_StructName("Unnamed Struct"),
  m_DataSize(0u),
  m_DataMemberNames(),
  m_DataIndices(),
  m_DataOffsets(),
  m_Created(false),
  m_DataPtr(nullptr)
{
}

void UniformBlock::SetBlockName(const std::string& BlockName) noexcept
{
  m_BlockName = BlockName;
}

void UniformBlock::SetStructName(const std::string& StructName) noexcept
{
  m_StructName = StructName;
}

void UniformBlock::SetDataSize(size_t DataSize) noexcept
{
  m_DataSize = DataSize;
}

void UniformBlock::AddDataMember(const std::string& DataMemberName) noexcept
{
  m_DataMemberNames.emplace_back(DataMemberName);
}

void UniformBlock::Create(GLuint ProgramID) noexcept
{
  // Verify everything is set up correctly
  if (m_BlockName == "Unnamed Block")
  {
    //TODO: Go through and have all errors match this syntax
    Log::Error("[UniformBlock::Create] - Trying to generate an unnamed Uniform Block");
    return;
  }

  // Retrieve the Index of the UBO
  m_UBOIndex = glGetUniformBlockIndex(ProgramID, m_BlockName.c_str());

  // Retrieve the size of the program's uniform block
  GLint uboSize = -1;
  glGetActiveUniformBlockiv(ProgramID, m_UBOIndex, GL_UNIFORM_BLOCK_DATA_SIZE, &uboSize);

  // Verify this is the size of the local data structure
  if (static_cast<size_t>(uboSize) != m_DataSize)
  {
    Log::Error("[UniformBlock::Create] - Invalid Data Size vs Program's Block Size");
    return;
  }

  size_t count = m_DataMemberNames.size();

  std::vector<const GLchar*> names(count);
  for (const string& name : m_DataMemberNames)
  {
    names.push_back(static_cast<const GLchar*>(name.c_str()));
  }

  GLuint* indices = new GLuint[count];
  GLint* offsets = new GLint[count];

  glGetUniformIndices(ProgramID, static_cast<GLsizei>(count), &names.data()[0], indices);
  glGetActiveUniformsiv(ProgramID, static_cast<GLsizei>(count), indices, GL_UNIFORM_OFFSET, offsets);

  //TODO: Gotta be a better way to do this
  for (int i = 0; i < count; ++i)
  {
    m_DataIndices.push_back(indices[i]);
    m_DataOffsets.push_back(offsets[i]);
  }

  delete[] indices;
  delete[] offsets;

  // Generate the buffers
  glGenBuffers(1, &m_UBOIndex);
  glBindBuffer(GL_UNIFORM_BUFFER, m_UBOIndex);
  glBufferData(GL_UNIFORM_BUFFER, m_DataSize, NULL, GL_DYNAMIC_DRAW);
  glBindBufferRange(GL_UNIFORM_BUFFER, 0, m_UBOIndex, 0, m_DataSize);

  Log::Trace("Uniform Block " + m_BlockName + " successfully created");
  m_Created = true;
}

// TODO: Explore the option of templatizing this
void UniformBlock::SetDataPointer(void* DataPtr)
{
  if (DataPtr == nullptr)
  {
    Log::Error("[UniformBlock::SetDataPointer] - DataPtr parameter is nullptr");
    return;
  }

  m_DataPtr = DataPtr;
}

void UniformBlock::SendData() noexcept
{
  if (m_DataPtr == nullptr)
  {
    Log::Error("[UniformBlock::SendData] - Data pointer is nullptr");
    return;
  }

  glBindBuffer(GL_UNIFORM_BUFFER, m_UBOIndex);
  glBufferData(GL_UNIFORM_BUFFER, m_DataSize, m_DataPtr, GL_DYNAMIC_DRAW);
  glBindBuffer(GL_UNIFORM_BUFFER, 0);
}
