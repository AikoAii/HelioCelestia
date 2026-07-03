#pragma once

#include "heliocelestia/io/render_style.hpp"
#include "heliocelestia/models/lunar_position.hpp"
#include "heliocelestia/models/solar_position.hpp"
#include "heliocelestia/models/sunrise_result.hpp"

#include <optional>
#include <string>

namespace heliocelestia::io {

std::string formatSolarPosition(
    const models::SolarPosition& position,
    const std::optional<models::SunriseResult>& sunrise,
    RenderStyle style = {},
    const std::optional<models::LunarPosition>& moon = std::nullopt
);

} // namespace heliocelestia::io
