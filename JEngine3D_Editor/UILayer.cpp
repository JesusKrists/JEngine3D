#include "UILayer.hpp"

#include <JEngine3D/Core/Base.hpp>
#include <JEngine3D/Core/Application.hpp>
#include <JEngine3D/Debug/View/IImGuiDebugView.hpp>
#include <JEngine3D/Core/ImGui/ImGuiLayer.hpp>
#include <JEngine3D/Core/Types.hpp>
#include <JEngine3D/Renderer/IFramebuffer.hpp>
#include <JEngine3D/Renderer/ITexture.hpp>
#include <JEngine3D/Renderer/Renderer2D.hpp>
#include <JEngine3D/Renderer/IRendererObjectCreator.hpp>

#include <imgui.h>
#include <stb_image.h>

#include <filesystem>

#include <Tracy.hpp>

// IWYU pragma: no_include <glm/detail/type_vec3.inl>
// IWYU pragma: no_include <glm/detail/type_vec4.inl>
// IWYU pragma: no_include <glm/ext/vector_float3.hpp>

namespace JE {
class IEvent;
}

namespace JEditor {


void UILayer::OnCreate()
{
  ZoneScopedN("UILayer::OnCreate");// NOLINT
  JE::ForEach(JE_APP.DebugViews(), [](JE::IImGuiDebugView &view) { view.Open(); });

  LoadImGuiSettings();

  constexpr uint32_t WHITE_COLOR = 0xFFFFFFFF;
  m_TestTexture = JE::IRendererObjectCreator::Get().CreateTexture();
  m_TestTexture->SetData("UILayer test texture",
    { reinterpret_cast<const std::byte *>(&WHITE_COLOR), 4 },// NOLINT
    { 1, 1 },
    JE::TextureFormat::RGBA8);


  stbi_set_flip_vertically_on_load(1);
  JE::Size2DI imageSize = { 0, 0 };
  int imageChannels = 0;
  unsigned char *data =
    stbi_load("assets/textures/testtexture.jpg", &imageSize.Width, &imageSize.Height, &imageChannels, 4);
  m_MemeTexture = JE::IRendererObjectCreator::Get().CreateTexture("assets/textures/testtexture.jpg",
    { reinterpret_cast<const std::byte *>(data), static_cast<size_t>(imageSize.Width * imageSize.Height * 4) },// NOLINT
    imageSize,
    JE::TextureFormat::RGBA8);
  stbi_image_free(data);

  m_GameViewportFBO = JE::IRendererObjectCreator::Get().CreateFramebuffer({ m_GameViewportSize,// NOLINT
    { JE::FramebufferAttachmentFormat::RGBA8, JE::FramebufferAttachmentFormat::DEPTH24STENCIL8 } });
}

void UILayer::OnDestroy() {}

void UILayer::OnUpdate()
{
  ZoneScopedN("UILayer::OnUpdate");// NOLINT
  auto UpdateImGuiLayer = [&]() {
    if (m_ResetDockLayout) {
      ImGui::LoadIniSettingsFromDisk("assets/imgui/default_layout.ini");
      m_ResetDockLayout = false;
    }

    if (ImGui::IsKeyDown(ImGui::GetKeyIndex(ImGuiKey_Escape)) && !JE_APP.ImGuiLayer().CaptureEvents()) {
      JE_APP.ImGuiLayer().SetCaptureEvents(true);
    }

    if (m_GameViewportFBO->Configuration().Size != m_GameViewportSize) {
      m_GameViewportFBO->Resize(m_GameViewportSize);
    }
  };
  UpdateImGuiLayer();

  auto Renderer2DTest = [&]() {
    ZoneScopedN("Renderer2D Test");// NOLINT

    static constexpr auto CLEAR_COLOR = JE::Color{ 0.1F, 0.1F, 0.1F, 1.0F };

    auto &rendererAPI = JE_APP.RendererAPI();
    auto &renderer2D = JE_APP.Renderer2D();

    rendererAPI.SetClearColor(CLEAR_COLOR);
    rendererAPI.Clear();

    m_GameViewportFBO->Bind();
    rendererAPI.Clear();
    m_GameViewportFBO->Unbind();


    renderer2D.BeginBatch(m_GameViewportFBO.get());

    auto vertex0 = JE::Vertex{ glm::vec3{ -0.5F, 0.0F, 0.0F }, JE::Color{ 1.0F, 0.0F, 0.0F, 1.0F } };// NOLINT
    auto vertex1 = JE::Vertex{ glm::vec3{ 0.5F, 0.0F, 0.0F }, JE::Color{ 0.0F, 1.0F, 0.0F, 1.0F } };// NOLINT
    auto vertex2 = JE::Vertex{ glm::vec3{ 0.0F, 1.0F, 0.0F }, JE::Color{ 0.0F, 0.0F, 1.0F, 1.0F } };// NOLINT

    auto vertex3 = JE::Vertex{ glm::vec3{ -0.5F, -1.0F, 0.0F }, JE::Color{ 1.0F, 0.0F, 0.0F, 1.0F } };// NOLINT
    auto vertex4 = JE::Vertex{ glm::vec3{ 0.5F, -1.0F, 0.0F }, JE::Color{ 0.0F, 1.0F, 0.0F, 1.0F } };// NOLINT
    auto vertex5 = JE::Vertex{ glm::vec3{ 0.0F, 0.0F, 0.0F }, JE::Color{ 0.0F, 0.0F, 1.0F, 1.0F } };// NOLINT

    constexpr auto position = glm::vec3{ -0.80F, -0.80F, 0.0F };
    constexpr auto size = glm::vec2{ 0.15F, 0.15F };
    constexpr auto color = JE::Color{ 1.0F, 0.0F, 1.0F, 1.0F };

    // constexpr auto position2 = glm::vec3{ 0.0F, 0.0F, 0.0F };
    constexpr auto color2 = JE::Color{ 1.0F, 1.0F, 1.0F, 1.0F };

    renderer2D.DrawTriangle(vertex0, vertex1, vertex2, *m_TestTexture);
    renderer2D.DrawTriangle(vertex3, vertex4, vertex5);
    renderer2D.DrawQuad(position, size, color);
    for (int y = 0; y < 10; y++) {// NOLINT
      for (int x = 0; x < 10; x++) {// NOLINT
        const auto newPosition =
          glm::vec3{ -0.9F + (static_cast<float>(x) / 5), -0.9F + (static_cast<float>(y) / 5), 0 };
        renderer2D.DrawQuad(newPosition, size, *m_MemeTexture, color2);
      }
    }

    renderer2D.EndBatch();
  };

  Renderer2DTest();
}

void UILayer::OnImGuiRender()
{
  ZoneScopedN("UILayer::OnImGuiRender");// NOLINT
  ImGui::DockSpaceOverViewport(ImGui::GetMainViewport(), ImGuiDockNodeFlags_PassthruCentralNode);

  RenderMainMenuBar();
  RenderGameViewport();

  ImGui::ShowDemoWindow();
}

void UILayer::OnEvent(JE::IEvent &event) { JE::UNUSED(event); }

void UILayer::LoadImGuiSettings()// NOLINT(readability-convert-member-functions-to-static)
{
  if (!std::filesystem::exists("imgui.ini")) { ImGui::LoadIniSettingsFromDisk("assets/imgui/default_layout.ini"); }

  auto &imguiIO = ImGui::GetIO();

  // Load default font
  imguiIO.Fonts->AddFontDefault();

  ImFontConfig fontConfig;
  auto *font = imguiIO.Fonts->AddFontFromFileTTF("assets/fonts/SEGOEUI.TTF", 16.0f, &fontConfig);// NOLINT
  imguiIO.Fonts->Build();
  imguiIO.FontDefault = font;
}

void UILayer::RenderMainMenuBar()// NOLINT(readability-convert-member-functions-to-static)
{
  ZoneScopedN("UILayer::RenderMainMenuBar");// NOLINT
  auto RenderMenuBarDebugViews = [&]() {
    auto &views = JE_APP.DebugViews();

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
  ZoneScopedN("UILayer::RenderGameViewport");// NOLINT
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

    m_GameViewportSize = { static_cast<int32_t>(size.x), static_cast<int32_t>(size.y) };

    ImGui::Image(m_GameViewportFBO->AttachmentRendererID(),
      ImVec2{ static_cast<float>(m_GameViewportFBO->Configuration().Size.Width),
        static_cast<float>(m_GameViewportFBO->Configuration().Size.Height) },
      { 0, 1 },
      { 1, 0 });

    if (!JE_APP.ImGuiLayer().CaptureEvents()) {
      ImGui::PushClipRect(absoluteCursorStart, absoluteCursorEnd, false);
      ImGui::GetWindowDrawList()->AddRect(
        absoluteCursorStart, absoluteCursorEnd, IM_COL32(255, 255, 255, 127));// NOLINT
      ImGui::PopClipRect();
    }

    if (ImGui::IsItemClicked()) { JE_APP.ImGuiLayer().SetCaptureEvents(false); }


    ImGui::EndChild();
  }
  ImGui::End();
  ImGui::PopStyleVar();
  ImGui::PopStyleColor(3);
}

}// namespace JEditor