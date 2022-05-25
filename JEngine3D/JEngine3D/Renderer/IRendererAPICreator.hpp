#pragma once

#include "JEngine3D/Core/Assert.hpp"
#include "JEngine3D/Core/MemoryController.hpp"// for Scope, MemoryTag

namespace JE {

class IRendererAPI;

// NOLINTNEXTLINE(hicpp-special-member-functions, cppcoreguidelines-special-member-functions)
class IRendererAPICreator
{
  friend class Application;

public:
  IRendererAPICreator()
  {
    ASSERT(!s_RendererAPICreatorInstance, "RendererAPI Creator instance already created");
    s_RendererAPICreatorInstance = this;
  }

  [[nodiscard]] static inline auto Get() -> IRendererAPICreator &
  {
    ASSERT(s_RendererAPICreatorInstance, "RendererAPI Creator instance is null");
    return *s_RendererAPICreatorInstance;
  }

  virtual ~IRendererAPICreator() { s_RendererAPICreatorInstance = nullptr; };

private:
  [[nodiscard]] virtual auto CreateAPI() -> Scope<IRendererAPI, MemoryTag::Renderer> = 0;

  static IRendererAPICreator *s_RendererAPICreatorInstance;// NOLINT
};

}// namespace JE