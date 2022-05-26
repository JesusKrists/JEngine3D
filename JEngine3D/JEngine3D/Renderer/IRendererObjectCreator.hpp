#pragma once

#include "JEngine3D/Core/Assert.hpp"
#include "JEngine3D/Core/MemoryController.hpp"// for Scope, MemoryTag

namespace JE {

class IRendererAPI;
class ITexture;

// NOLINTNEXTLINE(hicpp-special-member-functions, cppcoreguidelines-special-member-functions)
class IRendererObjectCreator
{
  friend class Application;

public:
  IRendererObjectCreator()
  {
    ASSERT(!s_RendererObjectCreatorInstance, "Renderer Object Creator instance already created");
    s_RendererObjectCreatorInstance = this;
  }

  [[nodiscard]] static inline auto Get() -> IRendererObjectCreator &
  {
    ASSERT(s_RendererObjectCreatorInstance, "Renderer Object Creator instance is null");
    return *s_RendererObjectCreatorInstance;
  }

  virtual ~IRendererObjectCreator() { s_RendererObjectCreatorInstance = nullptr; };


  [[nodiscard]] virtual auto CreateTexture() -> Scope<ITexture, MemoryTag::Renderer> = 0;

private:
  [[nodiscard]] virtual auto CreateAPI() -> Scope<IRendererAPI, MemoryTag::Renderer> = 0;

  static IRendererObjectCreator *s_RendererObjectCreatorInstance;// NOLINT
};

}// namespace JE