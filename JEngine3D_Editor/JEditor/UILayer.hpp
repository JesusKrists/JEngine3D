#pragma once

#include <JEngine3D/Core/ILayer.hpp>
#include <JEngine3D/Core/MemoryController.hpp>
#include <JEngine3D/Renderer/ITexture.hpp>
#include <JEngine3D/Renderer/IFramebuffer.hpp>

#include "UI/IPanel.hpp"

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
  void InitializeUI();

  // cppcheck-suppress functionStatic
  void LoadImGuiSettings();
  void RenderMainMenuBar();
  void RenderGameViewport();

  JE::Vector<JE::Scope<IPanel, JE::MemoryTag::Editor>, JE::MemoryTag::Editor> m_UIPanels;

  bool m_ResetDockLayout = false;

  JE::Scope<JE::ITexture2D, JE::MemoryTag::Renderer> m_TestTexture;
  JE::Scope<JE::ITexture2D, JE::MemoryTag::Renderer> m_MemeTexture;
  JE::Scope<JE::IFramebuffer, JE::MemoryTag::Renderer> m_GameViewportFBO;
  JE::Size2DI m_GameViewportSize = { 1280, 720 };// NOLINT
};

}// namespace JEditor