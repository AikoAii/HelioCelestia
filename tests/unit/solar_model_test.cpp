#include "heliocelestia/astronomy/solar_model.hpp"

#include <cassert>

int main()
{
    using namespace heliocelestia;

    const core::DateTime dateTime {
        .year = 2026,
        .month = 5,
        .day = 22,
        .hour = 12,
        .minute = 0,
        .second = 0.0,
        .timezoneOffsetHours = 0.0
    };

    const models::EquatorialCoordinate coordinate =
        astronomy::
        calculateSolarEquatorialCoordinate(
            dateTime
        );

    //
    // RA should normalize correctly.
    //

    assert(
        coordinate.rightAscensionDegrees >= 0.0
    );

    assert(
        coordinate.rightAscensionDegrees < 360.0
    );

    //
    // Declination physical limits.
    //

    assert(
        coordinate.declinationDegrees >= -90.0
    );

    assert(
        coordinate.declinationDegrees <= 90.0
    );

    return 0;
}