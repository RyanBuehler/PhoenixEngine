//------------------------------------------------------------------------------
// File:    Component.h
// Author:  Ryan Buehler
// Created: May 04, 2022
// Desc:    The base class for game object components
//------------------------------------------------------------------------------
#pragma once

class GameObject;

class Component
{
  public:
  enum class Type : size_t
  {
    MESH,
    COUNT
  };

  Component() :
    m_bIsActive(false)
  {
    m_ID = m_ComponentCount++;
  }

  virtual ~Component() = default;
  Component(const Component&) = delete;
  Component& operator=(const Component&) = delete;
  Component(Component&&) = delete;
  Component& operator=(Component&&) = delete;

  [[nodiscard]] unsigned GetID() const noexcept { return m_ID; }
  [[nodiscard]] virtual Type GetType() const noexcept = 0;

  void SetIsActive(const bool bIsActive) noexcept { m_bIsActive = bIsActive; }
  [[nodiscard]] bool IsActive() const noexcept { return m_bIsActive; }

  private:
  static inline unsigned m_ComponentCount{ 0u }; // Class reference counter

  unsigned m_ID; // Current component ID#
  //TODO: GameObject& m_Parent;

  bool m_bIsActive;
};
