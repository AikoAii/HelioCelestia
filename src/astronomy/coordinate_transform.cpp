#include "heliocelestia/astronomy/coordinate_transform.hpp"

#include "heliocelestia/math/angle.hpp"

#include <cmath>

namespace heliocelestia::astronomy {

models::HorizontalCoordinate
transformEquatorialToHorizontal(
    const models::EquatorialCoordinate& equatorialCoordinate,

    core::Real localSiderealTimeDegrees,

    const models::Observer& observer
) noexcept
{
    //
    // Hour Angle (HA)
    //
    // Represents how far the object
    // has rotated across the local sky.
    //

    core::Real hourAngleDegrees =
        localSiderealTimeDegrees
        -
        equatorialCoordinate.rightAscensionDegrees;

    hourAngleDegrees =
        math::normalizeDegrees(
            hourAngleDegrees
        );

    //
    // Convert angles into radians
    // for trigonometric calculations.
    //

    const core::Real hourAngleRadians =
        math::degToRad(
            hourAngleDegrees
        );

    const core::Real declinationRadians =
        math::degToRad(
            equatorialCoordinate.declinationDegrees
        );

    const core::Real latitudeRadians =
        math::degToRad(
            observer.latitudeDegrees
        );

    //
    // Altitude calculation.
    //
    // Based on spherical astronomy
    // coordinate transformation.
    //

    const core::Real altitudeRadians =
        std::asin(
            (std::sin(declinationRadians)
            *
            std::sin(latitudeRadians))
            +
            (
                std::cos(declinationRadians)
                *
                std::cos(latitudeRadians)
                *
                std::cos(hourAngleRadians)
            )
        );

    //
    // Azimuth calculation.
    //

    core::Real azimuthRadians =
        std::atan2(
            -std::sin(hourAngleRadians),

            (
                std::tan(declinationRadians)
                *
                std::cos(latitudeRadians)
            )
            -
            (
                std::sin(latitudeRadians)
                *
                std::cos(hourAngleRadians)
            )
        );

    //
    // Convert results back into degrees.
    //

    const core::Real altitudeDegrees =
        math::radToDeg(
            altitudeRadians
        );

    core::Real azimuthDegrees =
        math::radToDeg(
            azimuthRadians
        );

    //
    // Azimuth is cyclic.
    //

    azimuthDegrees =
        math::normalizeDegrees(
            azimuthDegrees
        );

    return {
        .altitudeDegrees = altitudeDegrees,

        .azimuthDegrees = azimuthDegrees
    };
}

}