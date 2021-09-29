#include "pch.h"
#include "TestScene.h"
#include "Transform.h"

TestScene::TestScene() noexcept :
  IScene("Test Scene")
{
  Log::Trace("Test Scene Created");
}

void TestScene::OnLoad() noexcept
{
  Log::Trace("Test Scene Loaded");
  GameObject go;
  m_GameObjectArray.push_back(go);
}

void TestScene::OnInit() noexcept
{
  Log::Trace("Test Scene Initialized");
}

void TestScene::OnUpdate(float dt) noexcept
{
  GameObject& go = m_GameObjectArray[0];
  go.RotateY(0.5f);
}

void TestScene::OnShutdown() noexcept
{
  Log::Trace("Test Scene Shutdown");
}

void TestScene::OnUnload() noexcept
{
  Log::Trace("Test Scene Unloaded");
}