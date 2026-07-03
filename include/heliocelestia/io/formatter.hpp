#pragma once

#include "heliocelestia/io/render_style.hpp"
#include "heliocelestia/models/solar_position.hpp"
#include "heliocelestia/models/sunrise_result.hpp"

#include <optional>
#include <string>

namespace heliocelestia::io {

std::string formatSolarPosition(
    const models::SolarPosition& position,
    const std::optional<models::SunriseResult>& sunrise,
    RenderStyle style = {}
);

} // namespace heliocelestia::io
