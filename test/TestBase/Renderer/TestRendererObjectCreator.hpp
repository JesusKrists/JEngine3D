#pragma once

#include <JEngine3D/Core/MemoryController.hpp>
#include <JEngine3D/Renderer/IRendererObjectCreator.hpp>
#include <JEngine3D/Utility/ImageLoader.hpp>

#include "TestBuffer.hpp"
#include "TestVertexArray.hpp"
#include "TestTexture.hpp"
#include "TestShader.hpp"
#include "TestFramebuffer.hpp"
#include "TestRendererAPI.hpp"


class TestRendererObjectCreator final : public JE::IRendererObjectCreator
{
public:
    [[nodiscard]] inline auto CreateVertexBuffer(const JE::BufferLayout& layout)
    -> JE::Scope<JE::IVertexBuffer, JE::MemoryTag::Renderer> override
    {
        return JE::CreatePolymorphicScope<TestVertexBuffer, JE::MemoryTag::Renderer, JE::IVertexBuffer>(layout);
    }
    [[nodiscard]] inline auto CreateIndexBuffer() -> JE::Scope<JE::IIndexBuffer, JE::MemoryTag::Renderer> override
    {
        return JE::CreatePolymorphicScope<TestIndexBuffer, JE::MemoryTag::Renderer, JE::IIndexBuffer>();
    }
    [[nodiscard]] inline auto CreateIndexBuffer(const std::span<const uint32_t>& indices)
    -> JE::Scope<JE::IIndexBuffer, JE::MemoryTag::Renderer> override
    {
        return JE::CreatePolymorphicScope<TestIndexBuffer, JE::MemoryTag::Renderer, JE::IIndexBuffer>(indices);
    }
    [[nodiscard]] inline auto CreateVertexArray() -> JE::Scope<JE::IVertexArray, JE::MemoryTag::Renderer> override
    {
        return JE::CreatePolymorphicScope<TestVertexArray, JE::MemoryTag::Renderer, JE::IVertexArray>();
    }

    [[nodiscard]] inline auto CreateTexture(const std::string_view&           sourcePath,
                                            const std::span<const std::byte>& textureData,
                                            const JE::Size2DI&                textureDimensions,
                                            JE::TextureFormat format) -> JE::Scope<JE::ITexture2D, JE::MemoryTag::Renderer> override
    {
        return JE::CreatePolymorphicScope<TestTexture2D, JE::MemoryTag::Renderer, JE::ITexture2D>(
        sourcePath, textureData, textureDimensions, format);
    }
    [[nodiscard]] inline auto CreateTexture(const JE::Image& sourceImage) -> JE::Scope<JE::ITexture2D, JE::MemoryTag::Renderer> override
    {
        return JE::CreatePolymorphicScope<TestTexture2D, JE::MemoryTag::Renderer, JE::ITexture2D>(
        sourceImage.SourcePath, sourceImage.Data, sourceImage.Size, JE::ImageFormatToTextureFormat(sourceImage.Format));
    }
    [[nodiscard]] inline auto CreateTexture() -> JE::Scope<JE::ITexture2D, JE::MemoryTag::Renderer> override
    {
        return JE::CreatePolymorphicScope<TestTexture2D, JE::MemoryTag::Renderer, JE::ITexture2D>();
    }

    [[nodiscard]] inline auto
    CreateShader(const std::string_view& name, const std::string_view& vertexSource, const std::string_view& fragmentSource)
    -> JE::Scope<JE::IShader, JE::MemoryTag::Renderer> override
    {
        return JE::CreatePolymorphicScope<TestShader, JE::MemoryTag::Renderer, JE::IShader>(name, vertexSource, fragmentSource);
    }

    [[nodiscard]] inline auto CreateFramebuffer(const JE::FramebufferConfiguration& configuration)
    -> JE::Scope<JE::IFramebuffer, JE::MemoryTag::Renderer> override
    {
        return JE::CreatePolymorphicScope<TestFramebuffer, JE::MemoryTag::Renderer, JE::IFramebuffer>(configuration);
    }

private:
    [[nodiscard]] inline auto CreateAPI() -> JE::Scope<JE::IRendererAPI, JE::MemoryTag::Renderer> override
    {
        return JE::CreatePolymorphicScope<TestRendererAPI, JE::MemoryTag::Renderer, JE::IRendererAPI>();
    }
};