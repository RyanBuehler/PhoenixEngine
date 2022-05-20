//------------------------------------------------------------------------------
// File:    UniformBlockManager.cpp
// Author:  Ryan Buehler
// Created: January 27, 2022
// Desc:    Manages the production and containment of UniformBlocks
//------------------------------------------------------------------------------
#include "pch.h"
#include "UniformBlockManager.h"

UniformBlockManager::UniformBlockManager() noexcept
{
  Log::trace("Uniform Block Manager initialized");
}

unsigned UniformBlockManager::RegisterNewBlockPrint(const UniformBlockPrint& BlockPrint) noexcept
{
  for (const UniformBlockPrint& print : m_UniformBlockPrints)
  {
    if (print.BlockName == BlockPrint.BlockName)
    {
      Log::error("[UniformBlockManager.cpp] - This block name already exists");
      return Error::INVALID_INDEX;
    }
  }

  const auto index = static_cast<unsigned>(m_UniformBlockPrints.size());
  m_UniformBlockPrints.emplace_back(BlockPrint);
  return index;
}

unsigned UniformBlockManager::CreateNewBlock(const unsigned BlockPrintID, const GLuint ProgramID, void* DataPtr)
{
  if (DataPtr == nullptr)
  {
    Log::error("[UniformBlockManager.cpp] - Invalid data pointer");
    return Error::INVALID_INDEX;
  }

  if (ProgramID == Error::INVALID_INDEX)
  {
    Log::error("[UniformBlockManager.cpp] - Invalid Program ID");
    return Error::INVALID_INDEX;
  }

  unsigned index = static_cast<unsigned>(m_UniformBlocks.size());
  m_UniformBlocks.emplace_back(UniformBlock({
    BlockPrintID,
    ProgramID,
    Error::Handle::INVALID_HANDLE,
    Error::Handle::INVALID_HANDLE,
    DataPtr
    }));

  GLuint& uboIndex = m_UniformBlocks[index].UBO_ID;
  uboIndex = glGetUniformBlockIndex(ProgramID, "LightArray");
  uboIndex = glGetUniformBlockIndex(ProgramID, m_UniformBlockPrints[BlockPrintID].BlockName.c_str());

  GLint uboSize;
  glGetActiveUniformBlockiv(ProgramID, uboIndex, GL_UNIFORM_BLOCK_DATA_SIZE, &uboSize);
  if (uboSize != m_UniformBlockPrints[BlockPrintID].DataSize)
  {
    Log::error("[UniformBlockManager.cpp] - Invalid block size");
    return Error::INVALID_INDEX;
  }

  GLuint& uboBuffer = m_UniformBlocks[index].UBO_BufferID;

  glGenBuffers(1, &uboBuffer);
  glBindBuffer(GL_UNIFORM_BUFFER, uboBuffer);
  glBufferData(GL_UNIFORM_BUFFER, uboSize, NULL, GL_DYNAMIC_DRAW);
  glBindBufferRange(GL_UNIFORM_BUFFER, 0, uboBuffer, 0, uboSize);

  return index;
}

void UniformBlockManager::SendData(const unsigned BlockID) const noexcept
{
  glBindBuffer(GL_UNIFORM_BUFFER, m_UniformBlocks[BlockID].UBO_BufferID);
  const size_t size = m_UniformBlockPrints[m_UniformBlocks[BlockID].PrintID].DataSize;
  glBufferData(GL_UNIFORM_BUFFER, size, m_UniformBlocks[BlockID].DataPtr, GL_DYNAMIC_DRAW);
  glBindBuffer(GL_UNIFORM_BUFFER, 0);
}
