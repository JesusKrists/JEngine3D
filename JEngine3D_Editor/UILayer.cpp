#include "UILayer.hpp"

#include <JEngine3D/Core/Base.hpp>
#include <JEngine3D/Core/Application.hpp>
#include <JEngine3D/Debug/View/IImGuiDebugView.hpp>
#include <JEngine3D/Core/ImGui/ImGuiLayer.hpp>
#include <JEngine3D/Core/Types.hpp>
#include <JEngine3D/Renderer/Renderer2D.hpp>
#include <JEngine3D/Renderer/Software/SoftwareFrameBufferObject.hpp>

#include <imgui.h>

#include <filesystem>

// IWYU pragma: no_include <glm/detail/type_vec3.inl>
// IWYU pragma: no_include <glm/detail/type_vec4.inl>
// IWYU pragma: no_include <glm/ext/vector_float3.hpp>

namespace JE {
class IEvent;
}

namespace JEditor {

void UILayer::OnCreate()
{
  JE::ForEach(JE::Application::Get().DebugViews(), [](JE::IImGuiDebugView &view) { view.Open(); });

  LoadImGuiSettings();
}
void UILayer::OnDestroy() {}

void UILayer::OnUpdate()
{
  auto UpdateImGuiLayer = [&]() {
    if (m_ResetDockLayout) {
      ImGui::LoadIniSettingsFromDisk("assets/imgui/default_layout.ini");
      m_ResetDockLayout = false;
    }

    if (ImGui::IsKeyDown(ImGui::GetKeyIndex(ImGuiKey_Escape)) && !JE::Application::Get().ImGuiLayer().CaptureEvents()) {
      JE::Application::Get().ImGuiLayer().SetCaptureEvents(true);
    }

    if (m_ResizeGameViewport) {
      m_GameViewportFrameBufferObject.Resize(m_GameViewportSize);
      m_ResizeGameViewport = false;
    }
  };
  UpdateImGuiLayer();

  static constexpr auto CLEAR_COLOR = JE::Color{ 0.1F, 0.1F, 0.1F, 1.0F };
  // JE::Application::Get().MainWindow().GraphicsContext().Clear(CLEAR_COLOR);

  auto &renderer2D = JE::Application::Get().Renderer2D();

  if (m_GameViewportFrameBufferObject.Size() != JE::Size2DI{ 0, 0 }) {
    m_GameViewportFrameBufferObject.Clear(CLEAR_COLOR);
    renderer2D.BeginBatch(&m_GameViewportFrameBufferObject);

    constexpr auto vertex0 = JE::Vertex{ glm::vec3{ -0.5F, 0.0F, 0.0F }, JE::Color{ 1.0F, 0.0F, 0.0F, 1.0F } };
    constexpr auto vertex1 = JE::Vertex{ glm::vec3{ 0.5F, 0.0F, 0.0F }, JE::Color{ 0.0F, 1.0F, 0.0F, 1.0F } };
    constexpr auto vertex2 = JE::Vertex{ glm::vec3{ 0.0F, 1.0F, 0.0F }, JE::Color{ 0.0F, 0.0F, 1.0F, 1.0F } };

    constexpr auto vertex3 = JE::Vertex{ glm::vec3{ -0.5F, -1.0F, 0.0F }, JE::Color{ 1.0F, 0.0F, 0.0F, 1.0F } };
    constexpr auto vertex4 = JE::Vertex{ glm::vec3{ 0.5F, -1.0F, 0.0F }, JE::Color{ 0.0F, 1.0F, 0.0F, 1.0F } };
    constexpr auto vertex5 = JE::Vertex{ glm::vec3{ 0.0F, 0.0F, 0.0F }, JE::Color{ 0.0F, 0.0F, 1.0F, 1.0F } };

    constexpr auto position = glm::vec3{ -0.95F, -0.95F, 0.0F };
    constexpr auto size = glm::vec2{ 0.3F, 0.3F };
    constexpr auto color = JE::Color{ 1.0F, 0.0F, 1.0F, 1.0F };

    renderer2D.DrawTriangle(vertex0, vertex1, vertex2);
    renderer2D.DrawTriangle(vertex3, vertex4, vertex5);
    renderer2D.DrawQuad(position, size, color);

    renderer2D.EndBatch();
  }
}

void UILayer::OnImGuiRender()
{
  ImGui::DockSpaceOverViewport(ImGui::GetMainViewport(), ImGuiDockNodeFlags_PassthruCentralNode);
  RenderMainMenuBar();
  RenderGameViewport();

  ImGui::ShowDemoWindow();
}

void UILayer::OnEvent(JE::IEvent &event) { JE::UNUSED(event); }

void UILayer::LoadImGuiSettings()// NOLINT(readability-convert-member-functions-to-static)
{
  if (std::filesystem::exists("imgui.ini")) { return; }

  ImGui::LoadIniSettingsFromDisk("assets/imgui/default_layout.ini");
}

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
    if (ImGui::BeginMenu("Options")) {
      if (ImGui::MenuItem("Reset Dock Layout")) { m_ResetDockLayout = true; }
      ImGui::EndMenu();
    }
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
  ImGui::PushStyleColor(ImGuiCol_ResizeGrip, 0);
  ImGui::PushStyleColor(ImGuiCol_ResizeGripHovered, 0);
  ImGui::PushStyleColor(ImGuiCol_ResizeGripActive, 0);
  ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 2, 2 });
  ImGui::Begin("Game Viewport", nullptr);
  {
    // Using a Child allow to fill all the space of the window.
    // It also alows customization
    ImGui::BeginChild("GameRender");
    // Get the size of the child (i.e. the whole draw size of the windows).
    ImVec2 size = ImGui::GetWindowSize();
    ImVec2 absoluteCursorPos = ImGui::GetCursorScreenPos();
    ImVec2 absoluteCursorStart = { absoluteCursorPos.x - 1, absoluteCursorPos.y - 1 };
    ImVec2 absoluteCursorEnd = { absoluteCursorPos.x + size.x + 1, absoluteCursorPos.y + size.y + 1 };

    // TODO(JesusKrists): Super temporary software rasterizer stuff, replace with OpenGL stuff later

    const auto &FrameBufferSize = m_GameViewportFrameBufferObject.Size();
    if (static_cast<int32_t>(size.x) != FrameBufferSize.Width
        || static_cast<int32_t>(size.y) != FrameBufferSize.Height) {
      m_ResizeGameViewport = true;
      m_GameViewportSize = { static_cast<int32_t>(size.x), static_cast<int32_t>(size.y) };
    }


    m_ImGuiSWTextureWrapper =
      imgui_sw::Texture{ m_GameViewportFrameBufferObject.PixelPtr(), FrameBufferSize.Width, FrameBufferSize.Height };
    ImGui::Image(reinterpret_cast<ImTextureID>(&m_ImGuiSWTextureWrapper),// NOLINT
      ImVec2{ static_cast<float>(FrameBufferSize.Width), static_cast<float>(FrameBufferSize.Height) },
      ImVec2(0, 0),
      ImVec2(1, 1));// NOLINT

    if (!JE::Application::Get().ImGuiLayer().CaptureEvents()) {
      ImGui::PushClipRect(absoluteCursorStart, absoluteCursorEnd, false);
      ImGui::GetWindowDrawList()->AddRect(
        absoluteCursorStart, absoluteCursorEnd, IM_COL32(255, 255, 255, 127));// NOLINT
      ImGui::PopClipRect();
    }

    if (ImGui::IsItemClicked()) { JE::Application::Get().ImGuiLayer().SetCaptureEvents(false); }


    ImGui::EndChild();
  }
  ImGui::End();
  ImGui::PopStyleVar();
  ImGui::PopStyleColor(3);
}

}// namespace JEditor