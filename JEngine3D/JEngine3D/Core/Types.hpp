#pragma once

#include <cstdint>// IWYU pragma: export
#include <string>// IWYU pragma: export
#include <string_view>// IWYU pragma: export

namespace JE {

struct Size2D
{
  int32_t Width;
  int32_t Height;

  constexpr auto operator==(const Size2D &other) const -> bool = default;
};

}// namespace JE
