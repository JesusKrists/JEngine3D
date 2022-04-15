#include "ImGuiSoftwareRenderer.hpp"

#include "JEngine3D/Core/Types.hpp"// for Siz...
#include "JEngine3D/Platform/SDL/SDLSoftwareGraphicsContext.hpp"// for SDL...
#include "JEngine3D/Platform/IGraphicsContext.hpp"// for IGr...

#include <imgui_sw.hpp>

namespace JE {

void ImGuiSoftwareRenderer::Initialize() { imgui_sw::bind_imgui_painting(); }

void ImGuiSoftwareRenderer::RenderImGui(IGraphicsContext &context)
{
  auto *swContext = dynamic_cast<SDLSoftwareGraphicsContext *>(&context);
  if (swContext != nullptr) {
    context.MakeCurrent();
    imgui_sw::paint_imgui(
      static_cast<uint32_t *>(swContext->PixelPtr()), context.DrawableSize().Width, context.DrawableSize().Height);
  }
}

void ImGuiSoftwareRenderer::Destroy() { imgui_sw::unbind_imgui_painting(); }

}// namespace JE