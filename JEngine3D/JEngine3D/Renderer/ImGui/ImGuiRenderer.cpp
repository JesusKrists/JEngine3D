#include "ImGuiRenderer.hpp"

#include "JEngine3D/Core/Base.hpp"

#include <imgui.h>

namespace JE {

ImGuiRenderer::ImGuiRenderer() = default;

ImGuiRenderer::~ImGuiRenderer() = default;

void ImGuiRenderer::RenderDrawData(ImDrawData *drawData)
{
  for (int i = 0; i < drawData->CmdListsCount; ++i) {
    const auto *commandList = drawData->CmdLists[i];// NOLINT

    // Setup OpenGL state (glViewport)
    // Setup VAO stuff
    // Setup shader stuff (Projection etc.)

    RenderCommandList(commandList);
  }
}

void ImGuiRenderer::RenderCommandList(const ImDrawList *drawList)
{
  for (int i = 0; i < drawList->CmdBuffer.Size; ++i) {
    const auto &command = drawList->CmdBuffer[i];

    // Setup clip rect stuff
    // Bind Texture

    // DrawCall

    UNUSED(command);
  }
}

}// namespace JE