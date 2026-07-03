#pragma once

#include "heliocelestia/io/render_style.hpp"
#include "heliocelestia/models/solar_position.hpp"
#include "heliocelestia/models/sunrise_result.hpp"

#include <optional>

namespace heliocelestia::io {

class ConsoleRenderer {
public:
    explicit ConsoleRenderer(RenderStyle style = {});

    void renderSolarPosition(
        const models::SolarPosition& position,
        const std::optional<models::SunriseResult>& sunrise
    ) const;

private:
    RenderStyle style_;
};

} // namespace heliocelestia::io
