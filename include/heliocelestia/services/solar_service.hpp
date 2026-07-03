#pragma once

#include "heliocelestia/core/datetime.hpp"
#include "heliocelestia/models/lunar_position.hpp"
#include "heliocelestia/models/observer.hpp"
#include "heliocelestia/models/solar_position.hpp"
#include "heliocelestia/models/sunrise_result.hpp"

namespace heliocelestia::services {

class SolarService {
public:
    models::SolarPosition calculateSolarPosition(
        const models::Observer& observer,
        const core::DateTime& dateTime
    ) const noexcept;

    models::SunriseResult calculateSunriseSunset(
        const models::Observer& observer,
        const core::DateTime& date
    ) const noexcept;

    models::LunarPosition calculateLunarPosition(
        const models::Observer& observer,
        const core::DateTime& dateTime
    ) const noexcept;
};

} // namespace heliocelestia::services
