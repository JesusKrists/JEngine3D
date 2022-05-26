#pragma once

#include "JEngine3D/Core/Base.hpp"
#include "JEngine3D/Core/Types.hpp"

namespace JE {

// NOLINTNEXTLINE(hicpp-special-member-functions, cppcoreguidelines-special-member-functions)
class ITexture
{
public:
  virtual ~ITexture() = default;

  virtual void SetData(const uint32_t *data, const Size2DI &size) = 0;
  [[nodiscard]] virtual auto Size() const -> const Size2DI & = 0;

  virtual void Bind(uint32_t slot = 0) const = 0;

  [[nodiscard]] virtual auto RendererID() const -> size_t = 0;
};

}// namespace JE