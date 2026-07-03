#pragma once

#include "heliocelestia/core/types.hpp"

namespace heliocelestia::models {

struct SolarPosition {
    core::Real altitudeDegrees { 0.0 };
    core::Real altitudeCorrectedDegrees { 0.0 };
    core::Real azimuthDegrees { 0.0 };
    core::Real declinationDegrees { 0.0 };
    core::Real rightAscensionDegrees { 0.0 };
    core::Real julianDate { 0.0 };
};

} // namespace heliocelestia::models
