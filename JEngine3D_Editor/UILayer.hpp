#pragma once

#include <JEngine3D/Core/ILayer.hpp>
#include <JEngine3D/Renderer/ITexture.hpp>

namespace JE {
class IEvent;
}

namespace JEditor {

class UILayer final : public JE::ILayer
{
public:
  UILayer() : JE::ILayer("JEditor UI Layer") {}

  void OnCreate() override;
  void OnDestroy() override;

  void OnUpdate() override;
  void OnImGuiRender() override;

  void OnEvent(JE::IEvent &event) override;

private:
  // cppcheck-suppress functionStatic
  void LoadImGuiSettings();
  // cppcheck-suppress functionStatic
  void RenderMainMenuBar();
  void RenderGameViewport();

  bool m_ResetDockLayout = false;


  JE::Scope<JE::ITexture2D, JE::MemoryTag::Renderer> m_TestTexture;
  JE::Scope<JE::ITexture2D, JE::MemoryTag::Renderer> m_MemeTexture;

  bool m_ResizeGameViewport = false;
  JE::Size2DI m_GameViewportSize = { 0, 0 };
};

}// namespace JEditor