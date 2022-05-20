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

  [[nodiscard]] bool IsActive() const noexcept { return m_bIsActive; }
  void SetIsActive(const bool Value) noexcept { m_bIsActive = Value; }

  void OnEndFrame();
  [[nodiscard]] float GetFPS() const;
  [[nodiscard]] size_t GetFrameCount() const noexcept { return m_FrameCount; }

  private:
  static constexpr size_t HISTORY_SIZE = 256u;
  array<float, HISTORY_SIZE> m_FrameHistory;
  array<float, HISTORY_SIZE>::iterator m_FrameIterator;

  std::chrono::steady_clock m_Clock;
  std::chrono::time_point<std::chrono::steady_clock> m_LastFrameTime;

  bool m_bIsActive;

  unsigned m_FrameCount;
};
