#pragma once

#include "JEngine3D/Core/Types.hpp"

namespace JE::Math {

constexpr auto Map(float input, const Range &inputRange, const Range &outputRange) -> float
{
  return (input - inputRange.start) * (outputRange.end - outputRange.start) / (inputRange.end - inputRange.start)
         + outputRange.start;
}

}// namespace JE::Math