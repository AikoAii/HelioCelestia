#pragma once

#include "heliocelestia/core/datetime.hpp"
#include "heliocelestia/models/lunar_position.hpp"
#include "heliocelestia/models/observer.hpp"
#include "heliocelestia/models/solar_position.hpp"
#include "heliocelestia/models/sunrise_result.hpp"

#include <optional>
#include <string>

namespace heliocelestia::io {

std::string formatJson(
    const models::Observer& observer,
    const core::DateTime& dateTime,
    const models::SolarPosition& position,
    const std::optional<models::SunriseResult>& sunrise,
    const std::optional<models::LunarPosition>& moon = std::nullopt
);

} // namespace heliocelestia::io
