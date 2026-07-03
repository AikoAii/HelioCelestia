#include "heliocelestia/astronomy/lunar_model.hpp"

#include <cassert>

int main()
{
    using namespace heliocelestia;

    const core::DateTime dateTime {
        .year = 2025,
        .month = 6,
        .day = 21,
        .hour = 12,
        .minute = 0,
        .second = 0.0,
        .timezoneOffsetHours = 0.0
    };

    const astronomy::LunarEquatorialCoordinate coordinate =
        astronomy::calculateLunarEquatorialCoordinate(dateTime);

    assert(coordinate.equatorial.rightAscensionDegrees >= 0.0);
    assert(coordinate.equatorial.rightAscensionDegrees < 360.0);

    assert(coordinate.equatorial.declinationDegrees >= -90.0);
    assert(coordinate.equatorial.declinationDegrees <= 90.0);

    assert(coordinate.eclipticLongitudeDegrees >= 0.0);
    assert(coordinate.eclipticLongitudeDegrees < 360.0);

    assert(coordinate.eclipticLatitudeDegrees >= -6.0);
    assert(coordinate.eclipticLatitudeDegrees <= 6.0);

    assert(coordinate.distanceKilometers > 350000.0);
    assert(coordinate.distanceKilometers < 410000.0);

    return 0;
}
