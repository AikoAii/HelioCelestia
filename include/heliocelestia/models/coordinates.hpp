#pragma once

#include "heliocelestia/core/types.hpp"

namespace heliocelestia::models {

struct EquatorialCoordinate {
    core::Real rightAscensionDegrees { 0.0 };
    core::Real declinationDegrees { 0.0 };
};

struct HorizontalCoordinate {
    core::Real altitudeDegrees { 0.0 };
    core::Real azimuthDegrees { 0.0 };
};

} // namespace heliocelestia::models
