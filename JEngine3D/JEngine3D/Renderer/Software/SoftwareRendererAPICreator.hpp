#pragma once

#include "JEngine3D/Renderer/IRendererAPICreator.hpp"

#include "SoftwareRendererAPI.hpp"

namespace JE {

class SoftwareRendererAPICreator : public IRendererAPICreator
{
private:
  [[nodiscard]] inline auto CreateAPI() -> Scope<IRendererAPI, MemoryTag::Renderer> override
  {
    return CreatePolymorphicScope<SoftwareRendererAPI, MemoryTag::Renderer, IRendererAPI>();
  }
};

}// namespace JE