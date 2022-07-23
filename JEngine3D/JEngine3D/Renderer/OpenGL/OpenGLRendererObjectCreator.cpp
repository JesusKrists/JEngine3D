#include "OpenGLRendererObjectCreator.hpp"

#include "JEngine3D/Renderer/OpenGL/OpenGLBuffer.hpp"
#include "JEngine3D/Renderer/OpenGL/OpenGLVertexArray.hpp"
#include "JEngine3D/Renderer/OpenGL/OpenGLTexture.hpp"
#include "JEngine3D/Renderer/OpenGL/OpenGLRendererAPI.hpp"
#include "JEngine3D/Renderer/OpenGL/OpenGLShader.hpp"
#include "JEngine3D/Renderer/OpenGL/OpenGLFramebuffer.hpp"
#include <JEngine3D/Utility/ImageLoader.hpp>

namespace JE {

    auto OpenGLRendererObjectCreator::CreateVertexBuffer(const BufferLayout& layout) -> Scope<IVertexBuffer, MemoryTag::Renderer>
    {
        return CreatePolymorphicScope<OpenGLVertexBuffer, MemoryTag::Renderer, IVertexBuffer>(layout);
    }

    auto OpenGLRendererObjectCreator::CreateIndexBuffer() -> Scope<IIndexBuffer, MemoryTag::Renderer>
    {
        return CreatePolymorphicScope<OpenGLIndexBuffer, MemoryTag::Renderer, IIndexBuffer>();
    }

    auto OpenGLRendererObjectCreator::CreateIndexBuffer(const std::span<const uint32_t>& indices)
    -> Scope<IIndexBuffer, MemoryTag::Renderer>
    {
        return CreatePolymorphicScope<OpenGLIndexBuffer, MemoryTag::Renderer, IIndexBuffer>(indices);
    }

    auto OpenGLRendererObjectCreator::CreateVertexArray() -> Scope<IVertexArray, MemoryTag::Renderer>
    {
        return CreatePolymorphicScope<OpenGLVertexArray, MemoryTag::Renderer, IVertexArray>();
    }

    auto OpenGLRendererObjectCreator::CreateTexture(const std::string_view&           sourcePath,
                                                    const std::span<const std::byte>& textureData,
                                                    const Size2DI&                    textureDimensions,
                                                    TextureFormat                     format) -> Scope<ITexture2D, MemoryTag::Renderer>
    {
        return CreatePolymorphicScope<OpenGLTexture2D, MemoryTag::Renderer, ITexture2D>(sourcePath, textureData, textureDimensions, format);
    }

    auto OpenGLRendererObjectCreator::CreateTexture(const Image& sourceImage) -> Scope<ITexture2D, MemoryTag::Renderer>
    {
        return CreatePolymorphicScope<OpenGLTexture2D, MemoryTag::Renderer, ITexture2D>(
        sourceImage.SourcePath, sourceImage.Data, sourceImage.Size, ImageFormatToTextureFormat(sourceImage.Format));
    }

    auto OpenGLRendererObjectCreator::CreateTexture() -> Scope<ITexture2D, MemoryTag::Renderer>
    {
        return CreatePolymorphicScope<OpenGLTexture2D, MemoryTag::Renderer, ITexture2D>();
    }

    auto OpenGLRendererObjectCreator::CreateShader(const std::string_view& name,
                                                   const std::string_view& vertexSource,
                                                   const std::string_view& fragmentSource) -> Scope<IShader, MemoryTag::Renderer>
    {
        return CreatePolymorphicScope<OpenGLShader, MemoryTag::Renderer, IShader>(name, vertexSource, fragmentSource);
    }

    [[nodiscard]] auto OpenGLRendererObjectCreator::CreateFramebuffer(const FramebufferConfiguration& configuration)
    -> Scope<IFramebuffer, MemoryTag::Renderer>
    {
        return CreatePolymorphicScope<OpenGLFramebuffer, MemoryTag::Renderer, IFramebuffer>(configuration);
    }

    auto OpenGLRendererObjectCreator::CreateAPI() -> Scope<IRendererAPI, MemoryTag::Renderer>
    {
        return CreatePolymorphicScope<OpenGLRendererAPI, MemoryTag::Renderer, IRendererAPI>();
    }

}// namespace JE