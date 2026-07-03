#pragma once

#include "heliocelestia/core/datetime.hpp"
#include "heliocelestia/core/types.hpp"

namespace heliocelestia::astronomy {

core::Real calculateJulianDate(const core::DateTime& dateTime) noexcept;

} // namespace heliocelestia::astronomy
