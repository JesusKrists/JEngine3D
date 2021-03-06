#pragma once

#include "JEngine3D/Renderer/IRendererObjectCreator.hpp"
#include <JEngine3D/Renderer/ITexture.hpp>

namespace JE {

    class OpenGLRendererObjectCreator final : public IRendererObjectCreator
    {
    public:
        [[nodiscard]] auto CreateVertexBuffer(const BufferLayout& layout) -> Scope<IVertexBuffer, MemoryTag::Renderer> override;
        [[nodiscard]] auto CreateIndexBuffer() -> Scope<IIndexBuffer, MemoryTag::Renderer> override;
        [[nodiscard]] auto CreateIndexBuffer(const std::span<const uint32_t>& indices) -> Scope<IIndexBuffer, MemoryTag::Renderer> override;
        [[nodiscard]] auto CreateVertexArray() -> Scope<IVertexArray, MemoryTag::Renderer> override;

        [[nodiscard]] auto CreateTexture(const std::string_view&           sourcePath,
                                         const std::span<const std::byte>& textureData,
                                         const Size2DI&                    textureDimensions,
                                         TextureFormat                     format) -> Scope<ITexture2D, MemoryTag::Renderer> override;
        [[nodiscard]] auto CreateTexture(const Image& sourceImage) -> Scope<ITexture2D, MemoryTag::Renderer> override;
        [[nodiscard]] auto CreateTexture() -> Scope<ITexture2D, MemoryTag::Renderer> override;

        [[nodiscard]] auto CreateShader(const std::string_view& name,
                                        const std::string_view& vertexSource,
                                        const std::string_view& fragmentSource) -> Scope<IShader, MemoryTag::Renderer> override;

        [[nodiscard]] auto CreateFramebuffer(const FramebufferConfiguration& configuration)
        -> Scope<IFramebuffer, MemoryTag::Renderer> override;

    private:
        [[nodiscard]] auto CreateAPI() -> Scope<IRendererAPI, MemoryTag::Renderer> override;
    };

}// namespace JE