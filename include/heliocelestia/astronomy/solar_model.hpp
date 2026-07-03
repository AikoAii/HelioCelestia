#pragma once

#include "heliocelestia/core/datetime.hpp"
#include "heliocelestia/models/coordinates.hpp"

namespace heliocelestia::astronomy {

models::EquatorialCoordinate calculateSolarEquatorialCoordinate(
    const core::DateTime& dateTime
) noexcept;

} // namespace heliocelestia::astronomy
