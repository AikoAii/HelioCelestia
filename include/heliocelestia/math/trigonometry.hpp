#pragma once

#include "heliocelestia/core/types.hpp"

#include <algorithm>
#include <cmath>

namespace heliocelestia::math {

inline core::Real safeAsin(const core::Real value) noexcept
{
    return std::asin(std::clamp(value, -1.0, 1.0));
}

inline core::Real safeAcos(const core::Real value) noexcept
{
    return std::acos(std::clamp(value, -1.0, 1.0));
}

} // namespace heliocelestia::math
