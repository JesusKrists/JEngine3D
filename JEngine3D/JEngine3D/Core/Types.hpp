#pragma once

#include <cstdint>// IWYU pragma: export
#include <string>// IWYU pragma: export
#include <string_view>// IWYU pragma: export

namespace JE {

struct Size2DI
{
  int32_t Width;
  int32_t Height;

  constexpr auto operator==(const Size2DI &other) const -> bool = default;
};

struct Position2DI
{
  int32_t X;
  int32_t Y;

  constexpr auto operator==(const Position2DI &other) const -> bool = default;
};

}// namespace JE
