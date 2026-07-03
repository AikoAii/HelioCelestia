#pragma once

#include "heliocelestia/core/types.hpp"

namespace heliocelestia::astronomy {

core::Real calculateAtmosphericRefraction(
    core::Real geometricAltitudeDegrees
) noexcept;

} // namespace heliocelestia::astronomy
