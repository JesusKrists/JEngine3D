#include "UILayer.hpp"

#include <JEngine3D/Core/Base.hpp>
#include <JEngine3D/Core/Application.hpp>
#include <JEngine3D/Debug/View/IImGuiDebugView.hpp>

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
  auto &renderer2D = JE::Application::Get().Renderer2D();
  renderer2D.BeginBatch();

  constexpr auto vertex0 = JE::Vertex{ JE::Color{ { 1.0F, 0.0F, 0.0F, 1.0F } }, glm::vec3{ -1.0F, -1.0F, 0.0F } };
  constexpr auto vertex1 = JE::Vertex{ JE::Color{ { 0.0F, 1.0F, 0.0F, 1.0F } }, glm::vec3{ 1.0F, -1.0F, 0.0F } };
  constexpr auto vertex2 = JE::Vertex{ JE::Color{ { 0.0F, 0.0F, 1.0F, 1.0F } }, glm::vec3{ 0.5F, 1.0F, 0.0F } };
  renderer2D.DrawTriangle(vertex0, vertex1, vertex2);

  renderer2D.EndBatch();
}

void UILayer::OnImGuiRender()
{
  ImGui::DockSpaceOverViewport(ImGui::GetMainViewport(), ImGuiDockNodeFlags_PassthruCentralNode);
  RenderMainMenuBar();

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

}// namespace JEditor