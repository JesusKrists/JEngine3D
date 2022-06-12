#include "ImGuiRenderer.hpp"

#include "JEngine3D/Core/Base.hpp"
#include "JEngine3D/Core/Application.hpp"
#include "JEngine3D/Renderer/ITexture.hpp"
#include "JEngine3D/Core/Types.hpp"

#include <glm/ext/matrix_clip_space.hpp>
#include <imgui.h>

#include <Tracy.hpp>

namespace JE {

static constexpr std::string_view VERTEX_SHADER_SOURCE =
  "#version 330 core\n"
  "layout (location = 0) in vec2 a_Position;\n"
  "layout (location = 1) in vec2 a_UV;\n"
  "layout (location = 2) in vec4 a_Color;\n"
  "out vec4 Frag_Color;\n"
  "out vec2 Frag_UV;\n"
  "uniform mat4 u_OrthoProjection;\n"
  "void main()\n"
  "{\n"
  "    Frag_Color = a_Color;\n"
  "    Frag_UV = a_UV;\n"
  "    gl_Position = u_OrthoProjection * vec4(a_Position.xy, 0, 1);\n"
  "}\n";

static constexpr std::string_view FRAGMENT_SHADER_SOURCE =
  "#version 330 core\n"
  "layout (location = 0) out vec4 Out_Color;\n"
  "in vec4 Frag_Color;\n"
  "in vec2 Frag_UV;\n"
  "uniform sampler2D u_Texture;\n"
  "void main()\n"
  "{\n"
  "    Out_Color = Frag_Color * texture(u_Texture, Frag_UV);\n"
  "}\n";

ImGuiRenderer::ImGuiRenderer()
{
  ZoneScopedN("ImGuiRenderer::ImGuiRenderer");// NOLINT
  m_VertexArray->AddVertexBuffer(*m_VertexBuffer);
  m_VertexArray->SetIndexBuffer(*m_IndexBuffer);

  m_Shader =
    IRendererObjectCreator::Get().CreateShader("JEngine3D ImGui Shader", VERTEX_SHADER_SOURCE, FRAGMENT_SHADER_SOURCE);
}

void ImGuiRenderer::Initialize()
{
  ZoneScopedN("ImGuiRenderer::Initialize");// NOLINT
  auto &imguiIO = ImGui::GetIO();

  uint8_t *textureData = nullptr;
  int fontWidth = 0;
  int fontHeight = 0;
  imguiIO.Fonts->GetTexDataAsRGBA32(&textureData, &fontWidth, &fontHeight);

  m_FontTexture = IRendererObjectCreator::Get().CreateTexture("ImGui Default Font",
    { reinterpret_cast<const std::byte *>(textureData), static_cast<size_t>(fontWidth * fontHeight * 4) },// NOLINT
    { fontWidth, fontHeight },
    TextureFormat::RGBA8);
  imguiIO.Fonts->TexID = m_FontTexture->RendererID();
}

void ImGuiRenderer::RenderDrawData(const ImDrawData &drawData)
{
  ZoneScopedN("ImGuiRenderer::RenderDrawData");// NOLINT

  m_VertexArray->Reset();

  m_PreviousRendererState = JE_APP.RendererAPI().RendererState();

  SetupRenderState(drawData);

  for (int i = 0; i < drawData.CmdListsCount; ++i) {
    const auto &commandList = *drawData.CmdLists[i];// NOLINT

    // Setup VAO stuff
    m_VertexBuffer->SetData({ reinterpret_cast<const std::byte *>(commandList.VtxBuffer.Data),// NOLINT
      static_cast<size_t>(commandList.VtxBuffer.Size) * sizeof(ImDrawVert) });
    m_IndexBuffer->SetData({ reinterpret_cast<const uint32_t *>(commandList.IdxBuffer.Data),// NOLINT
      static_cast<size_t>(commandList.IdxBuffer.Size) });

    RenderCommandList(commandList, drawData);
  }

  m_Shader->Unbind();

  JE_APP.RendererAPI().SetRendererState(m_PreviousRendererState);

  m_VertexArray->Delete();
}

void ImGuiRenderer::SetupRenderState(const ImDrawData &drawData)
{
  ZoneScopedN("ImGuiRenderer::SetupRenderState");// NOLINT
  JE_APP.RendererAPI().SetRendererState(m_ImGuiRendererState);

  m_Shader->Bind();
  m_Shader->SetMat4("u_OrthoProjection",
    glm::ortho(drawData.DisplayPos.x,
      drawData.DisplayPos.x + drawData.DisplaySize.x,
      drawData.DisplayPos.y + drawData.DisplaySize.y,
      drawData.DisplayPos.y));
  m_Shader->SetInt("u_Texture", 0);
}

void ImGuiRenderer::RenderCommandList(const ImDrawList &drawList, const ImDrawData &drawData)
{
  for (int i = 0; i < drawList.CmdBuffer.Size; ++i) {
    const auto &command = drawList.CmdBuffer[i];

    if (command.UserCallback != nullptr) {
      // User callback, registered via ImDrawList::AddCallback()
      // (ImDrawCallback_ResetRenderState is a special callback value used by the user to request the renderer to reset
      // render state.)
      if (command.UserCallback == ImDrawCallback_ResetRenderState)// NOLINT
      {
        SetupRenderState(drawData);
      } else {
        command.UserCallback(&drawList, &command);
      }
    }

    // Project scissor/clipping rectangles into framebuffer space
    ImVec2 clip_min((command.ClipRect.x - drawData.DisplayPos.x) * drawData.FramebufferScale.x,
      (command.ClipRect.y - drawData.DisplayPos.y) * drawData.FramebufferScale.y);
    ImVec2 clip_max((command.ClipRect.z - drawData.DisplayPos.x) * drawData.FramebufferScale.x,
      (command.ClipRect.w - drawData.DisplayPos.y) * drawData.FramebufferScale.y);
    if (clip_max.x <= clip_min.x || clip_max.y <= clip_min.y) { continue; }

    JE_APP.RendererAPI().SetClipRect(
      { { static_cast<int>(clip_min.x),
          static_cast<int>((drawData.DisplaySize.y * drawData.FramebufferScale.y - clip_max.y)) },
        { static_cast<int>((clip_max.x - clip_min.x)), static_cast<int>((clip_max.y - clip_min.y)) } });

    // Bind Texture
    auto rendererID = command.GetTexID();// NOLINT
    JE_APP.RendererAPI().BindTexture(rendererID);

    // DrawCall
    JE_APP.RendererAPI().DrawIndexed(*m_VertexArray, command.ElemCount, command.IdxOffset);
  }
}

}// namespace JE