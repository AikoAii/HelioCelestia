#pragma once

#include "heliocelestia/core/datetime.hpp"

namespace heliocelestia::validation {

bool isLeapYear(int year) noexcept;
bool isValidDateTime(const core::DateTime& dateTime) noexcept;

} // namespace heliocelestia::validation
