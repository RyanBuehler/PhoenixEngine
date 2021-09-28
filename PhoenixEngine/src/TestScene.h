#pragma once
#include "IScene.h"

class TestScene : public IScene
{
public:

  TestScene() noexcept;
  ~TestScene() = default;
  TestScene(const TestScene&) = delete;
  TestScene& operator=(const TestScene&) = delete;
  TestScene(TestScene&&) = delete;
  TestScene& operator=(TestScene&&) = delete;

  void OnLoad() noexcept;
  void OnInit() noexcept;
  void OnUpdate() noexcept;
  void OnReset() noexcept;
  void OnUnload() noexcept;

private:

};
