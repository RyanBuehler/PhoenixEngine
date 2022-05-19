//------------------------------------------------------------------------------
// File:    RenderStats.cpp
// Author:  Ryan Buehler
// Created: May 04, 2022
// Desc:    Calculates and tabulates stats on the Rendering Engine
//------------------------------------------------------------------------------
#include "pch.h"
#include "RenderStats.h"
#include <chrono>

RenderStats::RenderStats() noexcept :
  m_FrameHistory({ 0.0f }),
  m_FrameIterator(m_FrameHistory.begin()),
  m_IsActive(true),
  m_FrameCount(0u)
{}

void RenderStats::OnEndFrame()
{
  std::chrono::duration<float, std::milli> lastFrameDuration = m_Clock.now() - m_LastFrameTime;
  m_LastFrameTime = m_Clock.now();
  if (++m_FrameIterator == m_FrameHistory.end())
  {
    m_FrameIterator = m_FrameHistory.begin();
  }
  *m_FrameIterator = lastFrameDuration.count();
  ++m_FrameCount;
}

float RenderStats::GetFPS() const
{
  float sum = 0.f;

  for (auto it = m_FrameHistory.begin(); it != m_FrameHistory.end(); ++it)
  {
    sum += *it;
  }

  return 1000.f / (sum / HISTSIZE);
}
