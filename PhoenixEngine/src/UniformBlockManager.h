//------------------------------------------------------------------------------
// File:    UniformBlockManager.h
// Author:  Ryan Buehler
// Created: January 27, 2022
// Desc:    Manages the production and containment of UniformBlocks
//------------------------------------------------------------------------------
#pragma once
#include "GraphicsCommon.h"

class UniformBlockManager
{
public:
  struct UniformBlockPrint
  {
    string BlockName = "InvalidBlockName";
    size_t DataSize = 0u;
  };

  UniformBlockManager() noexcept;
  ~UniformBlockManager() = default;
  UniformBlockManager(const UniformBlockManager&) = delete;
  UniformBlockManager& operator=(const UniformBlockManager&) = delete;
  UniformBlockManager(UniformBlockManager&&) = delete;
  UniformBlockManager& operator=(UniformBlockManager&&) = delete;

  unsigned RegisterNewBlockPrint(const UniformBlockPrint& BlockPrint) noexcept;
  unsigned CreateNewBlock(unsigned BlockPrintID, GLuint ProgramID, void* DataPtr);

  void SendData(unsigned BlockID) const noexcept;

private:
  struct UniformBlock
  {
    unsigned PrintID;
    GLuint ProgramID;
    GLuint UBO_ID;
    GLuint UBO_BufferID;
    void* DataPtr;
  };

  vector<UniformBlockPrint> m_UniformBlockPrints;
  vector<UniformBlock> m_UniformBlocks;
};
