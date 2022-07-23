#pragma once

#include "JEngine3D/Core/Types.hpp"

namespace JE::Math {

    constexpr auto Map(float input, const Range& inputRange, const Range& outputRange) -> float
    {
        return (input - inputRange.Start) * (outputRange.End - outputRange.Start) / (inputRange.End - inputRange.Start) + outputRange.Start;
    }

}// namespace JE::Math