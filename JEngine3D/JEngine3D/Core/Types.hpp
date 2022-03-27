#pragma once

#include <cstdint>

namespace JE {

struct Size2D
{
  int32_t Width;
  int32_t Height;

  constexpr auto operator==(const Size2D &other) const -> bool = default;
};

}// namespace JE
