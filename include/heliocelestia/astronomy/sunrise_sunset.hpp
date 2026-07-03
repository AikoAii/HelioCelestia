#pragma once

#include "heliocelestia/core/datetime.hpp"
#include "heliocelestia/models/observer.hpp"
#include "heliocelestia/models/sunrise_result.hpp"

namespace heliocelestia::astronomy {

models::SunriseResult calculateSunriseSunset(
    const models::Observer& observer,
    const core::DateTime& date
) noexcept;

} // namespace heliocelestia::astronomy
