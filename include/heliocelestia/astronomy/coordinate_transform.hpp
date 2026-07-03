#pragma once

#include "heliocelestia/core/types.hpp"
#include "heliocelestia/models/coordinates.hpp"
#include "heliocelestia/models/observer.hpp"

namespace heliocelestia::astronomy {

models::HorizontalCoordinate transformEquatorialToHorizontal(
    const models::EquatorialCoordinate& equatorialCoordinate,
    core::Real localSiderealTimeDegrees,
    const models::Observer& observer
) noexcept;

} // namespace heliocelestia::astronomy
