#include "UILayer.hpp"

#include <JEngine3D/Core/Base.hpp>
#include <JEngine3D/Core/Application.hpp>
#include <JEngine3D/Debug/View/IImGuiDebugView.hpp>
#include <JEngine3D/Platform/IGraphicsContext.hpp>

#include <imgui_sw.hpp>
#include <string>

#include <imgui.h>

namespace JE {
class IEvent;
}

namespace JEditor {

void UILayer::OnCreate()
{
  JE::ForEach(JE::Application::Get().DebugViews(), [](JE::IImGuiDebugView &view) { view.Open(); });
}
void UILayer::OnDestroy() {}

void UILayer::OnUpdate()
{

  static constexpr auto CLEAR_COLOR = JE::Color{ { 0.1F, 0.1F, 0.1F, 1.0F } };
  // JE::Application::Get().MainWindow().GraphicsContext().Clear(CLEAR_COLOR);

  auto &renderer2D = JE::Application::Get().Renderer2D();

  if (m_GameViewportFrameBufferObject.Size() != JE::Size2DI{ 0, 0 }) {
    m_GameViewportFrameBufferObject.Clear(CLEAR_COLOR);
    renderer2D.BeginBatch(&m_GameViewportFrameBufferObject);

    constexpr auto vertex0 = JE::Vertex{ JE::Color{ { 1.0F, 0.0F, 0.0F, 1.0F } }, glm::vec3{ -0.5F, -0.5F, 0.0F } };
    constexpr auto vertex1 = JE::Vertex{ JE::Color{ { 0.0F, 1.0F, 0.0F, 1.0F } }, glm::vec3{ 0.5F, -0.5F, 0.0F } };
    constexpr auto vertex2 = JE::Vertex{ JE::Color{ { 0.0F, 0.0F, 1.0F, 1.0F } }, glm::vec3{ 0.0F, 0.5F, 0.0F } };
    renderer2D.DrawTriangle(vertex0, vertex1, vertex2);

    renderer2D.EndBatch();
  }
}

void UILayer::OnImGuiRender()
{
  ImGui::DockSpaceOverViewport(ImGui::GetMainViewport(), ImGuiDockNodeFlags_PassthruCentralNode);
  RenderMainMenuBar();
  RenderGameViewport();

  ImGui::Begin("Test Tint Color");
  ImGui::DragFloat4("Tint Color", reinterpret_cast<float *>(&m_TintColor), 0.01F, 0.0F, 1.0F);// NOLINT
  ImGui::End();

  ImGui::ShowDemoWindow();
}

void UILayer::OnEvent(JE::IEvent &event) { JE::UNUSED(event); }

void UILayer::RenderMainMenuBar()// NOLINT(readability-convert-member-functions-to-static)
{

  auto RenderMenuBarDebugViews = [&]() {
    auto &views = JE::Application::Get().DebugViews();

    JE::ForEach(views, [](JE::IImGuiDebugView &view) {
      if (ImGui::MenuItem(view.Name().c_str(), nullptr, view.IsOpen())) {
        if (view.IsOpen()) {
          view.Close();
        } else {
          view.Open();
        }
      }
    });
  };

  if (ImGui::BeginMainMenuBar()) {
    if (ImGui::BeginMenu("Debug")) {
      if (ImGui::BeginMenu("Views")) {
        RenderMenuBarDebugViews();
        ImGui::EndMenu();
      }
      ImGui::EndMenu();
    }
    ImGui::EndMainMenuBar();
  }
}


void UILayer::RenderGameViewport()
{
  ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0, 0 });
  ImGui::Begin("Game Viewport", nullptr, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_AlwaysUseWindowPadding);
  {
    // Using a Child allow to fill all the space of the window.
    // It also alows customization
    ImGui::BeginChild("GameRender");
    // Get the size of the child (i.e. the whole draw size of the windows).
    ImVec2 size = ImGui::GetWindowSize();

    // TODO(JesusKrists): Super temporary software rasterizer stuff, replace with OpenGL stuff later

    const auto &FrameBufferSize = m_GameViewportFrameBufferObject.Size();
    if (static_cast<int32_t>(size.x) != FrameBufferSize.Width
        || static_cast<int32_t>(size.y) != FrameBufferSize.Height) {
      m_GameViewportFrameBufferObject.Resize({ static_cast<int32_t>(size.x), static_cast<int32_t>(size.y) });
      m_ImGuiSWTextureWrapper =
        imgui_sw::Texture{ m_GameViewportFrameBufferObject.PixelPtr(), FrameBufferSize.Width, FrameBufferSize.Height };
    }

    // Because I use the texture from OpenGL, I need to invert the V from the UV.
    ImGui::Image(reinterpret_cast<ImTextureID>(&m_ImGuiSWTextureWrapper),// NOLINT
      size,
      ImVec2(0, 0),
      ImVec2(1, 1),
      m_TintColor);
    ImGui::EndChild();
  }
  ImGui::End();
  ImGui::PopStyleVar();
}

}// namespace JEditor