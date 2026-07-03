#pragma once

#include "heliocelestia/core/types.hpp"

namespace heliocelestia::models {

struct LunarPosition {
    core::Real altitudeDegrees { 0.0 };
    core::Real azimuthDegrees { 0.0 };
    core::Real declinationDegrees { 0.0 };
    core::Real rightAscensionDegrees { 0.0 };
    core::Real eclipticLongitudeDegrees { 0.0 };
    core::Real eclipticLatitudeDegrees { 0.0 };
    core::Real distanceKilometers { 0.0 };
    core::Real julianDate { 0.0 };
};

} // namespace heliocelestia::models
