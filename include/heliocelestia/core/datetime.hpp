#pragma once

#include "heliocelestia/core/types.hpp"

namespace heliocelestia::core {

struct DateTime {
    int  year { 2000 };
    int  month { 1 };
    int  day { 1 };
    int  hour { 0 };
    int  minute { 0 };
    Real second { 0.0 };
    Real timezoneOffsetHours { 0.0 };
};

} // namespace heliocelestia::core
