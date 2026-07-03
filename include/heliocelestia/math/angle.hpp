#pragma once

#include "heliocelestia/core/constants.hpp"
#include "heliocelestia/core/types.hpp"

namespace heliocelestia::math {

core::Real normalizeDegrees(core::Real degrees) noexcept;
core::Real normalizeRadians(core::Real radians) noexcept;

constexpr core::Real degToRad(const core::Real degrees) noexcept
{
    return degrees * core::kPi / 180.0;
}

constexpr core::Real radToDeg(const core::Real radians) noexcept
{
    return radians * 180.0 / core::kPi;
}

} // namespace heliocelestia::math
