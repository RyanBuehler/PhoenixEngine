//------------------------------------------------------------------------------
// File:    RenderStats.h
// Author:  Ryan Buehler
// Created: May 04, 2022
// Desc:    Calculates and tabulates stats on the Rendering Engine
//------------------------------------------------------------------------------
#pragma once

class RenderStats
{
  public:
  RenderStats() noexcept;
  ~RenderStats() = default;
  RenderStats(const RenderStats&) = delete;
  RenderStats& operator=(const RenderStats&) = delete;
  RenderStats(RenderStats&&) = delete;
  RenderStats& operator=(RenderStats&&) = delete;

  inline bool IsActive() const noexcept { return m_IsActive; }
  inline void SetIsActive(bool value) noexcept { m_IsActive = value; }

  void OnEndFrame();
  float GetFPS() const;
  inline size_t GetFrameCount() const noexcept { return m_FrameCount; }

  private:
  static constexpr size_t HISTSIZE = 256u;
  array<float, HISTSIZE> m_FrameHistory;
  array<float, HISTSIZE>::iterator m_FrameIterator;

  std::chrono::steady_clock m_Clock;
  std::chrono::time_point<std::chrono::steady_clock> m_LastFrameTime;

  bool m_IsActive;

  unsigned m_FrameCount;
};
