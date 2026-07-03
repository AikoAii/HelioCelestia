#pragma once

#include "heliocelestia/core/datetime.hpp"
#include "heliocelestia/models/coordinates.hpp"
#include "heliocelestia/models/lunar_position.hpp"

namespace heliocelestia::astronomy {

struct LunarEquatorialCoordinate {
    models::EquatorialCoordinate equatorial {};
    core::Real eclipticLongitudeDegrees { 0.0 };
    core::Real eclipticLatitudeDegrees { 0.0 };
    core::Real distanceKilometers { 0.0 };
};

LunarEquatorialCoordinate calculateLunarEquatorialCoordinate(
    const core::DateTime& dateTime
) noexcept;

} // namespace heliocelestia::astronomy
