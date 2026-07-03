#include "heliocelestia/io/console_renderer.hpp"

#include "heliocelestia/io/formatter.hpp"

#include <iostream>

namespace heliocelestia::io {

ConsoleRenderer::ConsoleRenderer(RenderStyle style)
    : style_ { style }
{
}

void ConsoleRenderer::renderSolarPosition(
    const models::SolarPosition&                position,
    const std::optional<models::SunriseResult>& sunrise
) const
{
    std::cout << formatSolarPosition(position, sunrise, style_);
}

} // namespace heliocelestia::io
