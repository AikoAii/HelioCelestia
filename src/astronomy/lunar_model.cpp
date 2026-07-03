#include "heliocelestia/astronomy/lunar_model.hpp"

#include "heliocelestia/astronomy/julian.hpp"
#include "heliocelestia/math/angle.hpp"

#include <cmath>

namespace heliocelestia::astronomy {

LunarEquatorialCoordinate calculateLunarEquatorialCoordinate(
    const core::DateTime& dateTime
) noexcept
{
    //
    // Low-precision lunar position model suitable for terminal reports.
    // It uses the Moon's mean longitude, mean anomaly, and argument of
    // latitude, then applies the largest periodic terms.
    //
    // References:
    //   Jean Meeus, Astronomical Algorithms, Chapters 45 and 47
    //   Paul Schlyter, Computing planetary positions
    //
    // Expected accuracy is appropriate for UX display, not occultations,
    // eclipses, or high-precision astrometry.
    //

    const core::Real julianDate =
        calculateJulianDate(dateTime);

    const core::Real daysSinceJ2000 =
        julianDate - 2451545.0;

    const core::Real meanLongitudeDegrees =
        math::normalizeDegrees(218.316 + 13.176396 * daysSinceJ2000);

    const core::Real meanAnomalyDegrees =
        math::normalizeDegrees(134.963 + 13.064993 * daysSinceJ2000);

    const core::Real argumentOfLatitudeDegrees =
        math::normalizeDegrees(93.272 + 13.229350 * daysSinceJ2000);

    const core::Real meanAnomalyRadians =
        math::degToRad(meanAnomalyDegrees);

    const core::Real argumentOfLatitudeRadians =
        math::degToRad(argumentOfLatitudeDegrees);

    const core::Real eclipticLongitudeDegrees =
        math::normalizeDegrees(
            meanLongitudeDegrees
            + 6.289 * std::sin(meanAnomalyRadians)
        );

    const core::Real eclipticLatitudeDegrees =
        5.128 * std::sin(argumentOfLatitudeRadians);

    const core::Real distanceKilometers =
        385001.0 - 20905.0 * std::cos(meanAnomalyRadians);

    const core::Real longitudeRadians =
        math::degToRad(eclipticLongitudeDegrees);

    const core::Real latitudeRadians =
        math::degToRad(eclipticLatitudeDegrees);

    const core::Real obliquityRadians =
        math::degToRad(core::kEarthObliquityDegrees);

    const core::Real rightAscensionRadians =
        std::atan2(
            std::sin(longitudeRadians) * std::cos(obliquityRadians)
                - std::tan(latitudeRadians) * std::sin(obliquityRadians),
            std::cos(longitudeRadians)
        );

    const core::Real declinationRadians =
        std::asin(
            std::sin(latitudeRadians) * std::cos(obliquityRadians)
                + std::cos(latitudeRadians)
                    * std::sin(obliquityRadians)
                    * std::sin(longitudeRadians)
        );

    return {
        .equatorial = {
            .rightAscensionDegrees =
                math::normalizeDegrees(math::radToDeg(rightAscensionRadians)),
            .declinationDegrees =
                math::radToDeg(declinationRadians)
        },
        .eclipticLongitudeDegrees = eclipticLongitudeDegrees,
        .eclipticLatitudeDegrees = eclipticLatitudeDegrees,
        .distanceKilometers = distanceKilometers
    };
}

} // namespace heliocelestia::astronomy
