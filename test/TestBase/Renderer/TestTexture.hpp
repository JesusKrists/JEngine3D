#pragma once

#include <JEngine3D/Core/Base.hpp>
#include <JEngine3D/Renderer/ITexture.hpp>

class TestTexture2D final : public JE::ITexture2D
{
public:
    TestTexture2D() = default;
    TestTexture2D(const std::string_view&           sourcePath,
                  const std::span<const std::byte>& textureData,
                  const JE::Size2DI&                textureDimensions,
                  JE::TextureFormat                 format)
        : m_SourcePath(sourcePath), m_TextureSize(textureDimensions), m_Format(format)
    {
        JE::UNUSED(textureData);
    }

    inline void SetData(const std::string_view&           sourcePath,
                        const std::span<const std::byte>& data,
                        const JE::Size2DI&                textureDimensions,
                        JE::TextureFormat                 format) override
    {
        JE::UNUSED(data);
        m_SourcePath  = sourcePath;
        m_TextureSize = textureDimensions;
        m_Format      = format;
    }

    [[nodiscard]] inline auto Size() const -> const JE::Size2DI& override { return m_TextureSize; }
    [[nodiscard]] inline auto SourcePath() const -> const std::string& override { return m_SourcePath; }

    inline void Bind(uint8_t slot = 0) const override { JE::UNUSED(slot); }

    [[nodiscard]] inline auto RendererID() const -> uint32_t override { return 1; }// NOLINT

private:
    std::string       m_SourcePath;
    JE::Size2DI       m_TextureSize = { 0, 0 };
    JE::TextureFormat m_Format      = JE::TextureFormat::RGBA8;
};