#include "pch.h"
#include "TestScene.h"

TestScene::TestScene() noexcept :
  IScene("Test Scene")
{
  Log::Trace("Test Scene Created");
}

void TestScene::OnLoad() noexcept
{
  Log::Trace("Test Scene Loaded");
}

void TestScene::OnInit() noexcept
{
  Log::Trace("Test Scene Initialized");
}

void TestScene::OnUpdate(float dt) noexcept
{
}

void TestScene::OnShutdown() noexcept
{
  Log::Trace("Test Scene Shutdown");
}

void TestScene::OnUnload() noexcept
{
  Log::Trace("Test Scene Unloaded");
}
