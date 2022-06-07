#pragma once

#include "JEngine3D/Core/Base.hpp"
#include "JEngine3D/Core/Types.hpp"

#include <span>

namespace JE {

enum class TextureFormat { RGBA8, RGB8 };

// NOLINTNEXTLINE(hicpp-special-member-functions, cppcoreguidelines-special-member-functions)
class ITexture2D
{
public:
  virtual ~ITexture2D() = default;

  virtual void SetData(const std::string_view &sourcePath,
    const std::span<const uint8_t> &data,
    const Size2DI &textureDimensions,
    TextureFormat format) = 0;

  [[nodiscard]] virtual auto Size() const -> const Size2DI & = 0;
  [[nodiscard]] virtual auto SourcePath() const -> const std::string & = 0;

  virtual void Bind(uint8_t slot = 0) const = 0;

  [[nodiscard]] virtual auto RendererID() const -> uint32_t = 0;
};

}// namespace JE