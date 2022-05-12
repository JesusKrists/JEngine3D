#include <JEngine3D/Renderer/Software/SoftwareFrameBufferObject.hpp>
#include <JEngine3D/Core/ILayer.hpp>

#include <imgui.h>
#include <imgui_sw.hpp>

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
  void RenderMainMenuBar();
  void RenderGameViewport();

  // TODO(JesusKrists): Temporary software rasterizer stuff, replace with real OpenGL stuff later
  JE::SoftwareFrameBufferObject m_GameViewportFrameBufferObject;
  imgui_sw::Texture m_ImGuiSWTextureWrapper;
};

}// namespace JEditor