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
        .year = 2026,
        .month = 5,
        .day = 22,
        .hour = 12,
        .minute = 0,
        .second = 0.0,
        .timezoneOffsetHours = 7.0
    };

    services::SolarService service;

    const models::SolarPosition position =
        service.calculateSolarPosition(
            observer,
            dateTime
        );

    //
    // Physical validation.
    //

    assert(
        position.altitudeDegrees >= -90.0
    );

    assert(
        position.altitudeDegrees <= 90.0
    );

    assert(
        position.azimuthDegrees >= 0.0
    );

    assert(
        position.azimuthDegrees < 360.0
    );

    return 0;
}