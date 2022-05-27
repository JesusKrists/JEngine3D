#include "UILayer.hpp"
#include "JEngine3D/Renderer/Software/SoftwareRendererAPI.hpp"

#include <JEngine3D/Core/Base.hpp>
#include <JEngine3D/Core/Application.hpp>
#include <JEngine3D/Debug/View/IImGuiDebugView.hpp>
#include <JEngine3D/Core/ImGui/ImGuiLayer.hpp>
#include <JEngine3D/Core/Types.hpp>
#include <JEngine3D/Renderer/Renderer2D.hpp>
#include <JEngine3D/Renderer/Software/SoftwareFrameBufferObject.hpp>
#include <JEngine3D/Renderer/IRendererObjectCreator.hpp>
#include <JEngine3D/Renderer/Software/ISoftwareShader.hpp>

#include <imgui.h>
#include <stb_image.h>

#include <filesystem>

// IWYU pragma: no_include <glm/detail/type_vec3.inl>
// IWYU pragma: no_include <glm/detail/type_vec4.inl>
// IWYU pragma: no_include <glm/ext/vector_float3.hpp>

namespace JE {
class IEvent;
}

namespace JEditor {

class TestSoftwareShader : public JE::ISoftwareShader
{
public:
  auto VertexShader(const JE::Vertex &vertex, uint32_t index) -> glm::vec4 override
  {
    varying_Color[index] = &vertex.Color;// NOLINT
    varying_UV[index] = &vertex.UV;// NOLINT
    flat_TextureIndex = vertex.TextureIndex;
    return glm::vec4{ vertex.Position, 1.0F };
  }

  auto FragmentShader(const glm::vec3 &barycentric, uint32_t &pixelColorOut) -> bool override
  {
    if (flat_TextureIndex == -1) {
      pixelColorOut = JE::CalculateColorFromBarycentric(
        barycentric, varying_Color[0]->ToABGR8(), varying_Color[1]->ToABGR8(), varying_Color[2]->ToABGR8());
    } else {
      auto uv = JE::CalculateUVFromBarycentric(barycentric, *varying_UV[0], *varying_UV[1], *varying_UV[2]);// NOLINT
      const auto *texture = JE::SoftwareRendererAPI::GetTexture(static_cast<uint32_t>(flat_TextureIndex));

      auto pixelColor = JE::CalculateColorFromBarycentric(
        barycentric, varying_Color[0]->ToABGR8(), varying_Color[1]->ToABGR8(), varying_Color[2]->ToABGR8());

      pixelColorOut = JE::MultiplyColor(JE::SampleTexture(uv, *texture), pixelColor);
    }

    return true;
  }

  [[nodiscard]] auto Name() const -> const std::string & override { return m_Name; }

private:
  std::array<const JE::Color *, 3> varying_Color{};
  std::array<const glm::vec2 *, 3> varying_UV{};
  int32_t flat_TextureIndex = -1;

  const std::string m_Name = "TestShader";
};

static TestSoftwareShader s_TestShader;// NOLINT


void UILayer::OnCreate()
{
  JE::ForEach(JE_APP.DebugViews(), [](JE::IImGuiDebugView &view) { view.Open(); });

  LoadImGuiSettings();

  // Prevent first frame clear from asserting
  m_TestTexture = JE::IRendererObjectCreator::Get().CreateTexture();
  m_MemeTexture = JE::IRendererObjectCreator::Get().CreateTexture();

  constexpr uint32_t WHITE_COLOR = 0xFFFFFFFF;
  m_TestTexture->SetData(&WHITE_COLOR, { 1, 1 });


  stbi_set_flip_vertically_on_load(1);
  JE::Size2DI imageSize = { 0, 0 };
  int imageChannels = 0;
  unsigned char *data =
    stbi_load("assets/textures/testtexture.jpg", &imageSize.Width, &imageSize.Height, &imageChannels, 4);
  m_MemeTexture->SetData(reinterpret_cast<const uint32_t *>(data), imageSize);// NOLINT
  stbi_image_free(data);
}

void UILayer::OnDestroy() {}

void UILayer::OnUpdate()
{
  auto UpdateImGuiLayer = [&]() {
    if (m_ResetDockLayout) {
      ImGui::LoadIniSettingsFromDisk("assets/imgui/default_layout.ini");
      m_ResetDockLayout = false;
    }

    if (ImGui::IsKeyDown(ImGui::GetKeyIndex(ImGuiKey_Escape)) && !JE_APP.ImGuiLayer().CaptureEvents()) {
      JE_APP.ImGuiLayer().SetCaptureEvents(true);
    }
  };
  UpdateImGuiLayer();

  auto Renderer2DTest = [&]() {
    if (m_ResizeGameViewport) {
      m_GameViewportFBO.Resize(m_GameViewportSize);
      m_ResizeGameViewport = false;
    }

    static constexpr auto CLEAR_COLOR = JE::Color{ 0.1F, 0.1F, 0.1F, 1.0F };

    auto &rendererAPI = JE_APP.RendererAPI();
    auto &renderer2D = JE_APP.Renderer2D();

    rendererAPI.SetClearColor(CLEAR_COLOR);

    if (m_GameViewportFBO.Size() != JE::Size2DI{ 0, 0 }) {
      m_GameViewportFBO.Bind();
      rendererAPI.Clear();
      m_GameViewportFBO.Unbind();

      s_TestShader.Bind();

      renderer2D.BeginBatch(&m_GameViewportFBO);

      auto vertex0 = JE::Vertex{ glm::vec3{ -0.5F, 0.0F, 0.0F }, JE::Color{ 1.0F, 0.0F, 0.0F, 1.0F } };// NOLINT
      auto vertex1 = JE::Vertex{ glm::vec3{ 0.5F, 0.0F, 0.0F }, JE::Color{ 0.0F, 1.0F, 0.0F, 1.0F } };// NOLINT
      auto vertex2 = JE::Vertex{ glm::vec3{ 0.0F, 1.0F, 0.0F }, JE::Color{ 0.0F, 0.0F, 1.0F, 1.0F } };// NOLINT

      auto vertex3 = JE::Vertex{ glm::vec3{ -0.5F, -1.0F, 0.0F }, JE::Color{ 1.0F, 0.0F, 0.0F, 1.0F } };// NOLINT
      auto vertex4 = JE::Vertex{ glm::vec3{ 0.5F, -1.0F, 0.0F }, JE::Color{ 0.0F, 1.0F, 0.0F, 1.0F } };// NOLINT
      auto vertex5 = JE::Vertex{ glm::vec3{ 0.0F, 0.0F, 0.0F }, JE::Color{ 0.0F, 0.0F, 1.0F, 1.0F } };// NOLINT

      constexpr auto position = glm::vec3{ -0.80F, -0.80F, 0.0F };
      constexpr auto size = glm::vec2{ 0.5F, 0.5F };
      constexpr auto color = JE::Color{ 1.0F, 0.0F, 1.0F, 1.0F };

      constexpr auto position2 = glm::vec3{ 0.0F, 0.0F, 0.0F };
      constexpr auto color2 = JE::Color{ 1.0F, 1.0F, 1.0F, 1.0F };

      renderer2D.DrawTriangle(vertex0, vertex1, vertex2, *m_TestTexture);
      renderer2D.DrawTriangle(vertex3, vertex4, vertex5);
      renderer2D.DrawQuad(position, size, color);
      renderer2D.DrawQuad(position2, size, *m_MemeTexture, color2);

      renderer2D.EndBatch();

      s_TestShader.Unbind();
    }
  };

  Renderer2DTest();
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

    const auto &FrameBufferSize = m_GameViewportFBO.Size();
    if (static_cast<int32_t>(size.x) != FrameBufferSize.Width
        || static_cast<int32_t>(size.y) != FrameBufferSize.Height) {
      m_ResizeGameViewport = true;
      m_GameViewportSize = { static_cast<int32_t>(size.x), static_cast<int32_t>(size.y) };
    }


    m_ImGuiSWTextureWrapper =
      imgui_sw::Texture{ m_GameViewportFBO.PixelPtr(), FrameBufferSize.Width, FrameBufferSize.Height };
    ImGui::Image(reinterpret_cast<ImTextureID>(&m_ImGuiSWTextureWrapper),// NOLINT
      ImVec2{ static_cast<float>(FrameBufferSize.Width), static_cast<float>(FrameBufferSize.Height) });

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