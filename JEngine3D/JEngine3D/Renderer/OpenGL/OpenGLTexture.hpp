#pragma once

#include "JEngine3D/Core/Base.hpp"
#include "JEngine3D/Renderer/ITexture.hpp"

namespace JE {

// NOLINTNEXTLINE(hicpp-special-member-functions, cppcoreguidelines-special-member-functions)
class OpenGLTexture2D final : public ITexture2D
{
public:
  OpenGLTexture2D(const std::string_view &sourcePath,
    const std::span<const uint8_t> &textureData,
    const Size2DI &textureDimensions,
    TextureFormat format);

  virtual ~OpenGLTexture2D();// NOLINT

  void SetData(const std::string_view &sourcePath,
    const std::span<const uint8_t> &data,
    const Size2DI &textureDimensions,
    TextureFormat format) override;

  [[nodiscard]] inline auto Size() const -> const Size2DI & override { return m_TextureSize; }
  [[nodiscard]] inline auto SourcePath() const -> const std::string & override { return m_SourcePath; }

  void Bind(uint8_t slot = 0) const override;

  [[nodiscard]] inline auto RendererID() const -> uint32_t override { return m_RendererID; }// NOLINT

private:
  std::string m_SourcePath;
  uint32_t m_RendererID = 0;
  Size2DI m_TextureSize = { 0, 0 };
  TextureFormat m_Format;
};

}// namespace JE