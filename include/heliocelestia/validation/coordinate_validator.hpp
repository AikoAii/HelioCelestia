#pragma once

#include "heliocelestia/core/types.hpp"

namespace heliocelestia::validation {

bool isValidLatitude(core::Real latitudeDegrees) noexcept;
bool isValidLongitude(core::Real longitudeDegrees) noexcept;
bool isValidElevation(core::Real elevationMeters) noexcept;

} // namespace heliocelestia::validation
