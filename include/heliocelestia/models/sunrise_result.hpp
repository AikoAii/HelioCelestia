#pragma once

#include "heliocelestia/core/types.hpp"

#include <optional>

namespace heliocelestia::models {

struct SunriseResult {
    std::optional<core::Real> sunriseUtcHours { std::nullopt };
    std::optional<core::Real> sunsetUtcHours { std::nullopt };
    core::Real solarNoonUtcHours { 0.0 };
    bool isPolarDay { false };
    bool isPolarNight { false };
};

} // namespace heliocelestia::models
