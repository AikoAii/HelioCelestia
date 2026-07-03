#include "heliocelestia/services/solar_service.hpp"

#include <cassert>

int main()
{
    using namespace heliocelestia;

    const models::Observer observer {
        .latitudeDegrees = -6.2088,
        .longitudeDegrees = 106.8456,
        .elevationMeters = 8.0,
        .timezoneOffsetHours = 7.0
    };

    const core::DateTime dateTime {
        .year = 2025,
        .month = 6,
        .day = 21,
        .hour = 21,
        .minute = 0,
        .second = 0.0,
        .timezoneOffsetHours = 7.0
    };

    services::SolarService service;

    const models::LunarPosition position =
        service.calculateLunarPosition(observer, dateTime);

    assert(position.altitudeDegrees >= -90.0);
    assert(position.altitudeDegrees <= 90.0);

    assert(position.azimuthDegrees >= 0.0);
    assert(position.azimuthDegrees < 360.0);

    assert(position.rightAscensionDegrees >= 0.0);
    assert(position.rightAscensionDegrees < 360.0);

    assert(position.declinationDegrees >= -90.0);
    assert(position.declinationDegrees <= 90.0);

    assert(position.distanceKilometers > 350000.0);
    assert(position.distanceKilometers < 410000.0);

    return 0;
}
