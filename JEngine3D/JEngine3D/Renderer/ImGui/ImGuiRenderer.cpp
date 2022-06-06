#include "ImGuiRenderer.hpp"

#include "JEngine3D/Core/Base.hpp"
#include "JEngine3D/Core/Application.hpp"
#include "JEngine3D/Renderer/ITexture.hpp"
#include "JEngine3D/Core/Types.hpp"

#include <glm/ext/matrix_clip_space.hpp>
#include <imgui.h>

namespace JE {

ImGuiRenderer::ImGuiRenderer()
{
  m_VertexArray->AddVertexBuffer(*m_VertexBuffer);
  m_VertexArray->SetIndexBuffer(*m_IndexBuffer);
}

void ImGuiRenderer::Initialize()
{
  auto &imguiIO = ImGui::GetIO();

  uint8_t *textureData = nullptr;
  int fontWidth = 0;
  int fontHeight = 0;
  imguiIO.Fonts->GetTexDataAsRGBA32(&textureData, &fontWidth, &fontHeight);

  m_FontTexture->SetData({ textureData, static_cast<size_t>(fontWidth * fontHeight * 4) }, { fontWidth, fontHeight });
  imguiIO.Fonts->TexID = m_FontTexture.get();
}

void ImGuiRenderer::RenderDrawData(ImDrawData *drawData)
{
  RectangleI viewport = { { static_cast<int32_t>(drawData->DisplayPos.x),
                            static_cast<int32_t>(drawData->DisplayPos.y) },
    { static_cast<int32_t>(drawData->DisplaySize.x), static_cast<int32_t>(drawData->DisplaySize.y) } };

  SetupRenderState(viewport);

  // Will project scissor/clipping rectangles into framebuffer space
  ImVec2 clipScale = drawData->FramebufferScale;// (1,1) unless using retina display which are often (2,2)

  for (int i = 0; i < drawData->CmdListsCount; ++i) {
    const auto *commandList = drawData->CmdLists[i];// NOLINT

    // Setup VAO stuff
    m_VertexBuffer->SetData({ reinterpret_cast<const uint8_t *>(commandList->VtxBuffer.Data),// NOLINT
      static_cast<size_t>(commandList->VtxBuffer.Size) * sizeof(ImDrawVert) });
    m_IndexBuffer->SetData({ reinterpret_cast<const uint32_t *>(commandList->IdxBuffer.Data),// NOLINT
      static_cast<size_t>(commandList->IdxBuffer.Size) });

    RenderCommandList(commandList, viewport, clipScale);
  }

  // s_SoftwareShader.Unbind();
}

// NOLINTNEXTLINE
void ImGuiRenderer::SetupRenderState(const RectangleI &viewport)
{
  UNUSED(viewport);
  // Setup OpenGL state (glViewport)

  // Setup shader stuff (Projection etc.)
  // s_SoftwareShader.Bind();
  // s_SoftwareShader.CalculateOrtho(viewport);
}

// NOLINTNEXTLINE
void ImGuiRenderer::RenderCommandList(const ImDrawList *drawList, const RectangleI &viewport, const ImVec2 &clipScale)
{
  for (int i = 0; i < drawList->CmdBuffer.Size; ++i) {
    const auto &command = drawList->CmdBuffer[i];

    if (command.UserCallback != nullptr) {
      // User callback, registered via ImDrawList::AddCallback()
      // (ImDrawCallback_ResetRenderState is a special callback value used by the user to request the renderer to reset
      // render state.)
      if (command.UserCallback == ImDrawCallback_ResetRenderState)// NOLINT
      {
        SetupRenderState(viewport);
      } else {
        command.UserCallback(drawList, &command);
      }
    }

    // Project scissor/clipping rectangles into framebuffer space
    ImVec2 clip_min((command.ClipRect.x - static_cast<float>(viewport.Position.X)) * clipScale.x,
      (command.ClipRect.y - static_cast<float>(viewport.Position.Y)) * clipScale.y);
    ImVec2 clip_max((command.ClipRect.z - static_cast<float>(viewport.Position.X)) * clipScale.x,
      (command.ClipRect.w - static_cast<float>(viewport.Position.Y)) * clipScale.y);
    if (clip_max.x <= clip_min.x || clip_max.y <= clip_min.y) { continue; }

    // Apply scissor/clipping rectangle (Y is inverted in OpenGL)
    /*glScissor((int)clip_min.x,
      (int)((float)fb_height - clip_max.y),
      (int)(clip_max.x - clip_min.x),
      (int)(clip_max.y - clip_min.y));*/

    // Bind Texture
    const auto *texture = command.GetTexID();// NOLINT
    texture->Bind();

    // DrawCall
    JE_APP.RendererAPI().DrawIndexed(*m_VertexArray);
  }
}

}// namespace JE