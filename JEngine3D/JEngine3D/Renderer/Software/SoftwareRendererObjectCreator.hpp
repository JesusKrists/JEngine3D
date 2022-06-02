#pragma once

#include "JEngine3D/Renderer/IRendererObjectCreator.hpp"

#include "SoftwareRendererAPI.hpp"
#include "SoftwareTexture.hpp"

namespace JE {

class SoftwareRendererObjectCreator final : public IRendererObjectCreator
{
public:
  [[nodiscard]] inline auto CreateTexture() -> Scope<ITexture, MemoryTag::Renderer> override
  {
    return CreatePolymorphicScope<SoftwareTexture, MemoryTag::Renderer, ITexture>();
  }

private:
  [[nodiscard]] inline auto CreateAPI() -> Scope<IRendererAPI, MemoryTag::Renderer> override
  {
    return CreatePolymorphicScope<SoftwareRendererAPI, MemoryTag::Renderer, IRendererAPI>();
  }
};

}// namespace JE