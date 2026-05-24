#include "heliocelestia/astronomy/solar_model.hpp"

#include "heliocelestia/astronomy/julian.hpp"

#include "heliocelestia/math/angle.hpp"

#include <cmath>

namespace heliocelestia::astronomy {

models::EquatorialCoordinate
calculateSolarEquatorialCoordinate(
    const core::DateTime& dateTime
) noexcept
{
    //
    // Source:
    // Jean Meeus
    // Astronomical Algorithms
    // Chapter 25
    //
    // Simplified solar position model.
    //

    //
    // Convert civil time into Julian Date.
    //

    const core::Real julianDate =
        calculateJulianDate(dateTime);

    //
    // Days since J2000 epoch.
    //

    const core::Real n =
        julianDate - 2451545.0;

    //
    // Mean solar longitude.
    //
    // Represents average angular position
    // of the Sun along the ecliptic.
    //

    core::Real meanLongitudeDegrees =
        280.460
        +
        (0.9856474 * n);

    meanLongitudeDegrees =
        math::normalizeDegrees(
            meanLongitudeDegrees
        );

    //
    // Mean anomaly.
    //
    // Represents Earth's orbital phase.
    //

    core::Real meanAnomalyDegrees =
        357.528
        +
        (0.9856003 * n);

    meanAnomalyDegrees =
        math::normalizeDegrees(
            meanAnomalyDegrees
        );

    //
    // Convert anomaly into radians
    // for trigonometric functions.
    //

    const core::Real meanAnomalyRadians =
        math::degToRad(
            meanAnomalyDegrees
        );

    //
    // Apparent ecliptic longitude.
    //

    core::Real eclipticLongitudeDegrees =
        meanLongitudeDegrees
        +
        (1.915 * std::sin(meanAnomalyRadians))
        +
        (0.020 * std::sin(2.0 * meanAnomalyRadians));

    eclipticLongitudeDegrees =
        math::normalizeDegrees(
            eclipticLongitudeDegrees
        );

    //
    // Earth's axial tilt (obliquity).
    //

    const core::Real obliquityRadians =
        math::degToRad(
            core::kEarthObliquityDegrees
        );

    //
    // Convert ecliptic longitude
    // into radians.
    //

    const core::Real eclipticLongitudeRadians =
        math::degToRad(
            eclipticLongitudeDegrees
        );

    //
    // Right Ascension (RA).
    //

    core::Real rightAscensionRadians =
        std::atan2(
            std::cos(obliquityRadians)
            *
            std::sin(eclipticLongitudeRadians),

            std::cos(eclipticLongitudeRadians)
        );

    //
    // Declination (Dec).
    //

    const core::Real declinationRadians =
        std::asin(
            std::sin(obliquityRadians)
            *
            std::sin(eclipticLongitudeRadians)
        );

    //
    // Convert results back into degrees.
    //

    core::Real rightAscensionDegrees =
        math::radToDeg(
            rightAscensionRadians
        );

    rightAscensionDegrees =
        math::normalizeDegrees(
            rightAscensionDegrees
        );

    const core::Real declinationDegrees =
        math::radToDeg(
            declinationRadians
        );

    return {
        .rightAscensionDegrees =
            rightAscensionDegrees,

        .declinationDegrees =
            declinationDegrees
    };
}

}