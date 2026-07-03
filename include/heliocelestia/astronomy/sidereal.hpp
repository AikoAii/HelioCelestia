#pragma once

#include "heliocelestia/core/types.hpp"

namespace heliocelestia::astronomy {

core::Real calculateGreenwichSiderealTime(core::Real julianDate) noexcept;
core::Real calculateLocalSiderealTime(
    core::Real greenwichSiderealTimeDegrees,
    core::Real longitudeDegrees
) noexcept;

} // namespace heliocelestia::astronomy
