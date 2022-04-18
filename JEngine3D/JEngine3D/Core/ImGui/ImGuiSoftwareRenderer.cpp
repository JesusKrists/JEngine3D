#include "ImGuiSoftwareRenderer.hpp"

#include "JEngine3D/Core/Application.hpp"
#include "JEngine3D/Core/Types.hpp"// for Siz...
#include "JEngine3D/Core/WindowController.hpp"// for Siz...
#include "JEngine3D/Platform/IGraphicsContext.hpp"// for SDL...
#include "JEngine3D/Platform/SDL/SDLSoftwareGraphicsContext.hpp"// for SDL...

#include <imgui_sw.hpp>
#include <imgui.h>

namespace JE {

void ImGuiSoftwareRenderer::Initialize() { imgui_sw::bind_imgui_painting(); }

void ImGuiSoftwareRenderer::RenderImGui(Window &window, ImDrawData *drawData)
{
  auto *swContext =
    dynamic_cast<SDLSoftwareGraphicsContext *>(// NOLINT(cppcoreguidelines-pro-type-static-cast-downcast)
      &window.GraphicsContext());

  if (swContext != nullptr) {
    auto drawableSize = swContext->DrawableSize();
    if ((static_cast<float>(drawableSize.Width) != drawData->DisplaySize.x)
        || (static_cast<float>(drawableSize.Height) != drawData->DisplaySize.y)) {
      drawData->DisplaySize = ImVec2{ static_cast<float>(drawableSize.Width), static_cast<float>(drawableSize.Height) };
    }
    if (&window == &Application::Get().MainWindow()) { swContext->Clear(); }
    imgui_sw::paint_imgui(static_cast<uint32_t *>(swContext->PixelPtr()), drawData);
  }
}

void ImGuiSoftwareRenderer::Destroy() { imgui_sw::unbind_imgui_painting(); }

}// namespace JE