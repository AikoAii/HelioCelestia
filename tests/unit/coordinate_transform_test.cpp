#include "heliocelestia/astronomy/coordinate_transform.hpp"

#include <cassert>

int main()
{
    using namespace heliocelestia;

    const models::Observer observer {
        .latitudeDegrees = -6.2,
        .longitudeDegrees = 106.8,
        .elevationMeters = 0.0,
        .timezoneOffsetHours = 7.0
    };

    const models::EquatorialCoordinate coordinate {
        .rightAscensionDegrees = 120.0,
        .declinationDegrees = 20.0
    };

    const models::HorizontalCoordinate horizontal =
        astronomy::
        transformEquatorialToHorizontal(
            coordinate,
            180.0,
            observer
        );

    //
    // Physical altitude limits.
    //

    assert(
        horizontal.altitudeDegrees >= -90.0
    );

    assert(
        horizontal.altitudeDegrees <= 90.0
    );

    //
    // Azimuth cyclic limits.
    //

    assert(
        horizontal.azimuthDegrees >= 0.0
    );

    assert(
        horizontal.azimuthDegrees < 360.0
    );

    return 0;
}