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
    //SPRITE,
    //CAMERA,
    COUNT
  };
  public:

  Component(GameObject& parent) :
    m_Parent(parent),
    m_bIsActive(false),
    m_bIsDirty(true)
  {
    m_ID = ComponentCount++;
  };
  virtual ~Component() {};
  Component(const Component&) = delete;
  Component& operator=(const Component&) = delete;
  Component(Component&&) = delete;
  Component& operator=(Component&&) = delete;

  virtual unsigned GetID() const noexcept final { return m_ID; };
  virtual Component::Type GetType() const noexcept = 0;

  inline void SetIsActive(bool isActive) noexcept { m_bIsActive = isActive; }
  inline bool IsActive() const noexcept { return m_bIsActive; }

  private:
  static inline unsigned ComponentCount{ 0u }; // Class reference counter

  unsigned m_ID; // Current component ID#
  GameObject& m_Parent;

  bool m_bIsActive;
  bool m_bIsDirty;
};
