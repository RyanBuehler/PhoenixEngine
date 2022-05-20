//------------------------------------------------------------------------------
// File:    Renderer.h
// Author:  Ryan Buehler
// Created: 10/03/21
// Desc:    Handles all Rendering of the Phoenix Engine
//------------------------------------------------------------------------------
#pragma once
#include "RenderStats.h"
#include "ContextManager.h"
#include "ShaderManager.h"
#include "UniformBlockManager.h"
#include "GameObject.h"
#include "Camera.h"
#include "MeshManager.h"
#include "LightingSystem.h"
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
    FILL,
    WIREFRAME,
    COUNT
  };
  
public:
  /// <summary>
  /// Renderer Constructor
  /// </summary>
  /// <param name="DepthBufferEnabled">Should the depth buffer be enabled?</param>
  /// <param name="BackFaceCullEnabled">Should the back faces be culled?</param>
  Renderer(bool DepthBufferEnabled = true, bool BackFaceCullEnabled = false) noexcept;
  ~Renderer();

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
  void OnEndFrame() noexcept;

  /// <summary>
  /// Render a draw call with the given objects from the scene
  /// </summary>
  /// TODO: Is there any way to make this constant?
  /// <param name="GameObjects">A vector reference of game objects</param>
  /// TODO: Is there any way to make this constant?
  /// <param name="ActiveCamera">A reference to the active camera</param>
  void RenderScene(vector<GameObject>& GameObjects, Camera& ActiveCamera);

  void RenderFirstPass(vector<GameObject>& GameObjects);

  void RenderSecondPass(vector<GameObject>& GameObjects, Camera& ActiveCamera);

  /// <summary>
  /// Renders the skybox
  /// </summary>
  void RenderSkybox(Camera& ActiveCamera);

  /// <summary>
  /// Enables the Depth Buffer
  /// </summary>
  void EnableDepthBuffer() const noexcept;
  
  /// <summary>
  /// Disables the Depth buffer
  /// </summary>
  void DisableDepthBuffer() const noexcept;
  
  /// <summary>
  /// Check if the Depth Buffer is enabled
  /// </summary>
  /// <returns>T/F if the depth buffer is enabled</returns>
  static inline bool DepthBufferIsEnabled() noexcept;

  /// <summary>
  /// Enables back face culling
  /// </summary>
  void EnableBackFaceCull() const noexcept;

  /// <summary>
  /// Disables back face culling
  /// </summary>
  void DisableBackFaceCull() const noexcept;

  /// <summary>
  /// Check if the back face culling is enabled
  /// </summary>
  /// <returns>T/F if the back face culling is enabled</returns>
  static inline bool BackFaceCullIsEnabled() noexcept;

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
  /// <param name="GameObject"></param>
  void RenderGameObject(GameObject& GameObject);

#pragma region ImGui

#ifdef _IMGUI

  /// <summary>
  /// Temporary ImGui interface for rendering normals. This may or may not become a feature
  /// </summary>
  /// <param name="gameObject">The game object to which we want to render its mesh's normals</param>
  /// <param name="length">Length to draw the normals in world space</param>
  /// <param name="normalType">Vertex or face normals</param>
  //void RenderNormals(GameObject& gameObject, float length, Normals::Type normalType) noexcept;

#endif

#pragma endregion

  /// <summary>
  /// Loads the different rendering "contexts" or "how to draw a thing"
  /// </summary>
  /// <returns></returns>
  void LoadContexts() noexcept;
  
  /// <summary>
  /// Loads the "Blinn Phong" rendering context
  /// </summary>
  void LoadBlinnPhongContext() noexcept;
  
  /// <summary>
  /// Loads the "Debug" rendering context
  /// </summary>
  void LoadDebugContext() noexcept;

  /// <summary>
  /// Loads the "Skybox" rendering context
  /// </summary>
  void LoadSkyboxContext() noexcept;
  
  RenderStats m_RenderStats;

  ShaderManager m_ShaderManager;              // Handles shader related functionality
  ContextManager m_ContextManager;            // Handles and maintains the context information
  MeshManager m_MeshManager;                  // Handles and maintains the Mesh data
  UniformBlockManager m_UniformBlockManager;  // Handles and maintains the Uniform GPU Data

  LightingSystem m_Lighting;        // All lighting functionality

  Cubemap m_Skybox;                 // The 6-sided cubemap skybox

  GLint m_hSkyboxContext;          // The ID of the "Skybox" context
  //GLint m_hDiffuseContext;         // The ID of the "Diffuse" context
  //GLint m_hPhongLighting;          // The ID of the "PhongLighting" context
  //GLint m_hPhongShading;           // The ID of the "PhongShading" context
  GLint m_hBlinnPhong;             // The ID of the "BlinnPhong" context
  //GLint m_hPhongTexture;           // The ID of the "PhongTexture" context
  //GLint m_hReflection;             // The ID of the "Reflection" context
  GLint m_hDebugContext;           // The ID of the "Debug" context
  //GLint m_hBlinnPhongRefract;      // The ID of the "BlinnPhongRefract" context

  //TODO: Below for testing only
  unsigned m_LightingBlockPrintId;
  unsigned m_LightingBlockID;
  unsigned m_SkyboxMeshId{};

  EnvironmentMap m_EnvMap;

  //Texture diffTex;
  //Texture specTex;
};

