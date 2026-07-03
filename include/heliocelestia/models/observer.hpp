#pragma once

#include "heliocelestia/core/types.hpp"

namespace heliocelestia::models {

struct Observer {
    core::Real latitudeDegrees { 0.0 };
    core::Real longitudeDegrees { 0.0 };
    core::Real elevationMeters { 0.0 };
    core::Real timezoneOffsetHours { 0.0 };
};

} // namespace heliocelestia::models
