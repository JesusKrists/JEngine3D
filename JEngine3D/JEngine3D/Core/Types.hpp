#pragma once

#include <cstdint>

namespace JE {

struct Size2D
{
  int32_t width;
  int32_t height;

  auto operator==(const Size2D &other) const -> bool = default;
};

}// namespace JE
