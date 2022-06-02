#pragma once

#include "JEngine3D/Core/Base.hpp"
#include "JEngine3D/Core/Types.hpp"

#include <span>

namespace JE {

// NOLINTNEXTLINE(hicpp-special-member-functions, cppcoreguidelines-special-member-functions)
class ITexture
{
public:
  virtual ~ITexture() = default;

  virtual void SetData(const std::span<const uint8_t> &data, const Size2DI &textureDimensions) = 0;
  [[nodiscard]] virtual auto Size() const -> const Size2DI & = 0;

  virtual void Bind(uint32_t slot = 0) const = 0;

  [[nodiscard]] virtual auto PointerValue() const -> size_t = 0;
};

}// namespace JE