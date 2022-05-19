//------------------------------------------------------------------------------
// File:    EnvironmentMap
// Author:  Ryan Buehler
// Created: November 30, 2021
// Desc:    Captures a cube map around a location in the scene
//------------------------------------------------------------------------------
#include "pch.h"
#include "EnvironmentMap.h"

EnvironmentMap::EnvironmentMap() noexcept :
  m_hFBO(),
  m_hTexture(),
  m_Camera("Environment Map Camera")
{
  for (int i = 0; i < 6; ++i)
  {
    glGenFramebuffers(1, &m_hFBO[i]);
    glBindFramebuffer(GL_FRAMEBUFFER, m_hFBO[i]);

    m_hTexture[i] = Error::Handle::INVALID_HANDLE;
    m_hRBO[i] = Error::Handle::INVALID_HANDLE;

    glGenTextures(1, &m_hTexture[i]);
    glBindTexture(GL_TEXTURE_2D, m_hTexture[i]);

    //TODO: Resolution needs not be hardcoded
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1024, 1024, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_hTexture[i], 0);

    glGenRenderbuffers(1, &m_hRBO[i]);
    glBindRenderbuffer(GL_RENDERBUFFER, m_hRBO[i]);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 1024, 1024);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_hRBO[i]);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
      Log::Error("[EnvironmentMap] Framebuffer incomplete!");
      return;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0u);
  }

  Camera::ViewData viewData;
  viewData.FOV = 90.f;
  viewData.Aspect = 1.f;
  m_Camera.SetPosition({ 0.f, 0.f, 0.f });
  m_Camera.SetViewData(viewData);
}

EnvironmentMap::~EnvironmentMap()
{
  glDeleteFramebuffers(6, &m_hFBO[0]);
  glDeleteTextures(6, &m_hTexture[0]);
  glDeleteRenderbuffers(6, &m_hRBO[0]);
}

GLuint EnvironmentMap::GetFBOHandle(int i) const noexcept
{
  return m_hFBO[i];
}

GLuint EnvironmentMap::GetTextureHandle(GLuint index) const noexcept
{
  return m_hTexture[index];
}

Camera& EnvironmentMap::GetCamera() noexcept
{
  return m_Camera;
}

void EnvironmentMap::Bind(int i) noexcept
{
  glBindFramebuffer(GL_FRAMEBUFFER, m_hFBO[i]);
  glClearColor(1.0f, 0.f, 0.f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glEnable(GL_DEPTH_TEST);
}

void EnvironmentMap::EnableTextures() noexcept
{
  for (int i = 0; i < 6; ++i)
  {
    glActiveTexture(GL_TEXTURE0 + i);
    glBindTexture(GL_TEXTURE_2D, m_hTexture[i]);
  }
}
