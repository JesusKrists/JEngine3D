#include "UILayer.hpp"

#include <JEngine3D/Core/Base.hpp>
#include <JEngine3D/Core/Application.hpp>
#include <JEngine3D/Debug/View/IImGuiDebugView.hpp>

#include <string>

#include <imgui.h>

namespace JE {
class IEvent;
}

// static constexpr auto MILISECONDS = 1000;

namespace JEditor {

void UILayer::OnCreate()
{
  JE::ForEach(JE::Application::Get().DebugViews(), [](JE::IImGuiDebugView &view) { view.Open(); });
}
void UILayer::OnDestroy() {}

void UILayer::OnUpdate()
{
  // JE::Logger::ClientLogger().info("App delta time - {}", JE::Application::Get().DeltaTime() * MILISECONDS);
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