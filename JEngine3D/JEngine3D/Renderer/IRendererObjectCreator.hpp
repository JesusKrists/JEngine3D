#pragma once

#include "JEngine3D/Core/Assert.hpp"
#include "JEngine3D/Core/MemoryController.hpp"// for Scope, MemoryTag
#include "JEngine3D/Renderer/IBuffer.hpp"
#include "JEngine3D/Renderer/IShader.hpp"
#include "JEngine3D/Renderer/IVertexArray.hpp"
#include "JEngine3D/Renderer/ITexture.hpp"

namespace JE {

class IRendererAPI;

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


  [[nodiscard]] virtual auto CreateVertexBuffer(const BufferLayout &layout)
    -> Scope<IVertexBuffer, MemoryTag::Renderer> = 0;
  [[nodiscard]] virtual auto CreateIndexBuffer() -> Scope<IIndexBuffer, MemoryTag::Renderer> = 0;
  [[nodiscard]] virtual auto CreateIndexBuffer(const std::span<const uint32_t> &indices)
    -> Scope<IIndexBuffer, MemoryTag::Renderer> = 0;
  [[nodiscard]] virtual auto CreateVertexArray() -> Scope<IVertexArray, MemoryTag::Renderer> = 0;

  [[nodiscard]] virtual auto CreateTexture(const std::string_view &sourcePath,
    const std::span<const std::byte> &textureData,
    const Size2DI &textureDimensions,
    TextureFormat format) -> Scope<ITexture2D, MemoryTag::Renderer> = 0;

  [[nodiscard]] virtual auto CreateShader(const std::string_view &name,
    const std::string_view &vertexSource,
    const std::string_view &fragmentSource) -> Scope<IShader, MemoryTag::Renderer> = 0;

private:
  [[nodiscard]] virtual auto CreateAPI() -> Scope<IRendererAPI, MemoryTag::Renderer> = 0;

  static IRendererObjectCreator *s_RendererObjectCreatorInstance;// NOLINT
};

}// namespace JE