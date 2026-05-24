#include "heliocelestia/astronomy/julian.hpp"

#include <cassert>
#include <cmath>

int main()
{
    using namespace heliocelestia;

    //
    // Reference:
    // Meeus Chapter 7
    //
    // 2000 January 1.5
    // should produce:
    //
    // JD 2451545.0
    //

    const core::DateTime dateTime {
        .year = 2000,
        .month = 1,
        .day = 1,
        .hour = 12,
        .minute = 0,
        .second = 0.0,
        .timezoneOffsetHours = 0.0
    };

    const core::Real julianDate =
        astronomy::calculateJulianDate(
            dateTime
        );

    //
    // Floating-point tolerance.
    //

    constexpr core::Real epsilon =
        1e-6;

    assert(
        std::abs(
            julianDate - 2451545.0
        )
        <
        epsilon
    );

    return 0;
}