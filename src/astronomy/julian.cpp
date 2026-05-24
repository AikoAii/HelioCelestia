#include "heliocelestia/astronomy/julian.hpp"

#include <cmath>

namespace heliocelestia::astronomy {

core::Real calculateJulianDate(
    const core::DateTime& dateTime
) noexcept
{
    //
    // Source:
    // Jean Meeus
    // Astronomical Algorithms
    // Chapter 7
    //
    // Julian Date converts civil calendar time
    // into continuous astronomical days.
    //

    int year = dateTime.year;
    int month = dateTime.month;

    //
    // Convert local civil time into UTC fractional hours.
    //

    const core::Real utcHours =
        static_cast<core::Real>(dateTime.hour)
        +
        static_cast<core::Real>(dateTime.minute) / 60.0
        +
        dateTime.second / 3600.0
        -
        dateTime.timezoneOffsetHours;

    //
    // Meeus convention:
    //
    // January and February are treated
    // as months 13 and 14 of the previous year.
    //

    if (month <= 2) {
        year -= 1;
        month += 12;
    }

    //
    // Gregorian calendar correction.
    //

    const int a = year / 100;

    const int b =
        2
        -
        a
        +
        (a / 4);

    //
    // Fractional day contribution.
    //

    const core::Real day =
        static_cast<core::Real>(dateTime.day)
        +
        (utcHours / 24.0);

    //
    // Meeus Julian Date formula.
    //

    return
        std::floor(365.25 * (year + 4716))
        +
        std::floor(30.6001 * (month + 1))
        +
        day
        +
        static_cast<core::Real>(b)
        -
        1524.5;
}

}