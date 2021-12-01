//------------------------------------------------------------------------------
// File:    Renderer.h
// Author:  Ryan Buehler
// Created: 10/03/21
// Desc:    Handles all Renderering of the Phoenix Engine
//------------------------------------------------------------------------------
#pragma once
#include "ContextManager.h"
#include "ShaderManager.h"
#include "DebugRenderer.h"
#include "GameObject.h"
#include "Camera.h"
#include "MeshManager.h"
#include "LightingSystem.h"
#include "Texture.h"
#include "Cubemap.h"        //TODO: Include this?
#include "EnvironmentMap.h" //TODO: Include this?

class Renderer
{
public:
  /// <summary>
  /// Render Fill or Wireframe
  /// </summary>
  enum class RenderMode
  {
    Fill,
    Wireframe,
    COUNT
  };
public:
  /// <summary>
  /// Renderer Constructor
  /// </summary>
  /// <param name="depthBufferEnabled">Should the depth buffer be enabled?</param>
  /// <param name="backFaceCullEnabled">Should the back faces be culled?</param>
  Renderer(bool depthBufferEnabled = true, bool backFaceCullEnabled = false) noexcept;
  ~Renderer(); // Destructor

  // Rule of 5
  Renderer(const Renderer&) = delete;
  Renderer& operator=(const Renderer&) = delete;
  Renderer(Renderer&&) = delete;
  Renderer& operator=(Renderer&&) = delete;

  /// <summary>
  /// Called once at the beginning of the frame
  /// </summary>
  void OnBeginFrame() const noexcept;

  /// <summary>
  /// Called once at the end of the frame
  /// </summary>
  /// <returns></returns>
  void OnEndFrame() noexcept;

  /// <summary>
  /// Render a draw call with the given objects from the scene
  /// </summary>
  /// TODO: Is there any way to make this constant?
  /// <param name="gameObjects">A vector reference of game objects</param>
  /// TODO: Is there any way to make this constant?
  /// <param name="activeCamera">A reference to the active camera</param>
  void RenderScene(vector<GameObject>& gameObjects, Camera& activeCamera);

  void RenderFirstPass(vector<GameObject>& gameObjects);

  void RenderSecondPass(vector<GameObject>& gameObjects, Camera& activeCamera);

  /// <summary>
  /// Renders the skybox
  /// </summary>
  void RenderSkybox(Camera& activeCamera);

  /// <summary>
  /// Enables the Depth Buffer
  /// </summary>
  void EnableDepthBuffer() noexcept;
  
  /// <summary>
  /// Disables the Depth buffer
  /// </summary>
  void DisableDepthBuffer() noexcept;
  
  /// <summary>
  /// Check if the Depth Buffer is enabled
  /// </summary>
  /// <returns>T/F if the depth buffer is enabled</returns>
  inline bool DepthBufferIsEnabled() const noexcept;

  /// <summary>
  /// Enables back face culling
  /// </summary>
  void EnableBackFaceCull() noexcept;

  /// <summary>
  /// Disables back face culling
  /// </summary>
  void DisableBackFaceCull() noexcept;

  /// <summary>
  /// Check if the back face culling is enabled
  /// </summary>
  /// <returns>T/F if the back face culling is enabled</returns>
  inline bool BackFaceCullIsEnabled() const noexcept;

  /// <summary>
  /// Sets the render mode to Fill
  /// </summary>
  static void SetRenderModeFill() noexcept;

  /// <summary>
  /// Sets the render mode to wireframe only
  /// </summary>
  /// <returns></returns>
  static void SetRenderModeWireframe() noexcept;

private:
  /// <summary>
  /// Helper function to render a single game object
  /// </summary>
  /// <param name="gameObject"></param>
  void RenderGameObject(GameObject& gameObject);

#pragma region ImGui

#ifdef _IMGUI

  /// <summary>
  /// Temporary ImGui interface for rendering normals. This may or may not become a feature
  /// </summary>
  /// <param name="gameObject">The game object to which we want to render its mesh's normals</param>
  /// <param name="length">Length to draw the normals in world space</param>
  /// <param name="normalType">Vertex or face normals</param>
  void RenderNormals(GameObject& gameObject, float length, Normals::Type normalType) noexcept;

#endif

#pragma endregion

  /// <summary>
  /// Loads the different rendering "contexts" or "how to draw a thing"
  /// </summary>
  /// <returns></returns>
  void LoadContexts() noexcept;

  /// <summary>
  /// Loads the "Diffuse only" rendering context
  /// </summary>
  void LoadDiffuseContext() noexcept;

  /// <summary>
  /// Loads the "Phong Lighting" rendering context
  /// </summary>
  void LoadPhongLightingContext() noexcept;

  /// <summary>
  /// Loads the "Phong Shading" rendering context
  /// </summary>
  void LoadPhongShadingContext() noexcept;

  /// <summary>
  /// Loads the "Blinn Phong" rendering context
  /// </summary>
  void LoadBlinnPhongContext() noexcept;

  /// <summary>
  /// Loads the "Phong Texture" rendering context
  /// </summary>
  void LoadPhongTextureContext() noexcept;

  /// <summary>
  /// Loads the "Debug" rendering context
  /// </summary>
  void LoadDebugContext() noexcept;

  /// <summary>
  /// Loads the "Skybox" rendering context
  /// </summary>
  void LoadSkyboxContext() noexcept;

  /// <summary>
  /// Loads the "Reflection" rendering context
  /// </summary>
  void LoadReflectionContext() noexcept;

  ShaderManager m_ShaderManager;    // Handles shader related functionality
  ContextManager m_ContextManager;  // Handles and maintains the context information
  MeshManager m_MeshManager;        // Handles and maintains the Mesh data

  LightingSystem m_Lighting;        // All lighting functionality

  Cubemap m_Skybox;                 // The 6-sided cubemap skybox

  GLint m_hSkyboxContext;          // The ID of the "Skybox" context
  GLint m_hDiffuseContext;         // The ID of the "Diffuse" context
  GLint m_hPhongLighting;          // The ID of the "PhongLighting" context
  GLint m_hPhongShading;           // The ID of the "PhongShading" context
  GLint m_hBlinnPhong;             // The ID of the "BlinnPhong" context
  GLint m_hPhongTexture;           // The ID of the "PhongTexture" context
  GLint m_hReflection;             // The ID of the "Reflection" context
  GLint m_hDebugContext;           // The ID of the "Debug" context

  //TODO: Below for testing only
  unsigned SkyboxMeshID;

  EnvironmentMap envMap;

  GLint uboSize;
  GLuint uboIndex;
  GLuint uboBuffer;
  GLuint indices[5];
  GLint offsets[5];

  Texture diffTex;
  Texture specTex;
};

