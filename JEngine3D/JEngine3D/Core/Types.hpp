#pragma once

#include <cstdint>

namespace JE {

struct Size2D
{
  int32_t width;// cppcheck-suppress unusedStructMember
  int32_t height;// cppcheck-suppress unusedStructMember

  auto operator==(const Size2D &other) const -> bool = default;
};

}// namespace JE
