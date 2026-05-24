#include "heliocelestia/astronomy/sunrise_sunset.hpp"

#include "heliocelestia/astronomy/julian.hpp"

#include "heliocelestia/math/angle.hpp"
#include "heliocelestia/math/trigonometry.hpp"

#include <cmath>

namespace heliocelestia::astronomy {

models::SunriseResult calculateSunriseSunset(
    const models::Observer& observer,
    const core::DateTime&   date
) noexcept
{
    //
    // NOAA simplified sunrise/sunset algorithm.
    //
    // Source:
    //   https://en.wikipedia.org/wiki/Sunrise_equation
    //   Jean Meeus, Astronomical Algorithms, Chapter 15
    //
    // Only the date (year/month/day) portion of `date` is used.
    // We compute at UTC noon to keep the calculation on the
    // correct calendar day for all longitudes.
    //

    const core::DateTime utcNoon {
        .year                = date.year,
        .month               = date.month,
        .day                 = date.day,
        .hour                = 12,
        .minute              = 0,
        .second              = 0.0,
        .timezoneOffsetHours = 0.0
    };

    const core::Real jd = calculateJulianDate(utcNoon);

    //
    // Julian cycle count relative to J2000.
    // The 0.0008 offset centres the calculation at solar midnight.
    //

    const core::Real nStar = jd - 2451545.0 + 0.0008;

    //
    // Approximate Julian date of solar noon at the given longitude.
    //

    const core::Real jStar =
        nStar + (-observer.longitudeDegrees / 360.0);

    //
    // Solar mean anomaly (degrees).
    //

    const core::Real mDeg =
        math::normalizeDegrees(357.5291 + 0.98560028 * nStar);

    const core::Real mRad = math::degToRad(mDeg);

    //
    // Equation of center — first-order orbital correction.
    //

    const core::Real cDeg =
        1.9148 * std::sin(mRad)
        + 0.0200 * std::sin(2.0 * mRad)
        + 0.0003 * std::sin(3.0 * mRad);

    //
    // Ecliptic longitude of the Sun.
    //

    const core::Real lambdaDeg =
        math::normalizeDegrees(mDeg + cDeg + 180.0 + 102.9372);

    const core::Real lambdaRad = math::degToRad(lambdaDeg);

    //
    // Refined Julian date of solar transit (solar noon).
    //

    const core::Real jTransit =
        2451545.0
        + jStar
        + 0.0053 * std::sin(mRad)
        - 0.0069 * std::sin(2.0 * lambdaRad);

    //
    // Solar declination.
    //
    // J2000 mean obliquity approximation: ε ≈ 23.4397°
    //

    constexpr core::Real kObliquityRad =
        23.4397 * (3.14159265358979323846 / 180.0);

    const core::Real declinationRad =
        math::safeAsin(
            std::sin(lambdaRad) * std::sin(kObliquityRad)
        );

    //
    // Hour angle cosine for sunrise / sunset.
    //
    // The −0.8333° depression accounts for:
    //   ~0.5667° standard atmospheric refraction at horizon
    //   ~0.2667° apparent solar disk radius
    //

    constexpr core::Real kDepressionRad =
        -0.8333 * (3.14159265358979323846 / 180.0);

    const core::Real latRad =
        math::degToRad(observer.latitudeDegrees);

    const core::Real cosOmega0 =
        (std::sin(kDepressionRad) - std::sin(latRad) * std::sin(declinationRad))
        / (std::cos(latRad) * std::cos(declinationRad));

    //
    // Convert solar transit Julian date → UTC hours.
    //
    // JD epoch starts at noon (12:00 UTC), so:
    //   UTC hours = fmod(JD + 0.5, 1.0) × 24
    //

    auto jdToUtcHours = [](const core::Real jdValue) noexcept -> core::Real {
        core::Real hours = std::fmod(jdValue + 0.5, 1.0) * 24.0;
        if (hours < 0.0) hours += 24.0;
        return hours;
    };

    models::SunriseResult result;

    result.solarNoonUtcHours = jdToUtcHours(jTransit);

    //
    // Polar condition detection.
    //
    // cosOmega0 > 1  → argument of acos is out of range →
    //   the Sun never reaches the depression threshold → polar night.
    //
    // cosOmega0 < -1 → acos would give π (180°) →
    //   the Sun is always above the depression threshold → polar day.
    //

    if (cosOmega0 > 1.0) {

        result.isPolarNight      = true;
        result.sunriseUtcHours   = std::nullopt;
        result.sunsetUtcHours    = std::nullopt;

    } else if (cosOmega0 < -1.0) {

        result.isPolarDay        = true;
        result.sunriseUtcHours   = std::nullopt;
        result.sunsetUtcHours    = std::nullopt;

    } else {

        const core::Real omega0Deg =
            math::radToDeg(math::safeAcos(cosOmega0));

        const core::Real jRise =
            jTransit - (omega0Deg / 360.0);

        const core::Real jSet =
            jTransit + (omega0Deg / 360.0);

        result.sunriseUtcHours = jdToUtcHours(jRise);
        result.sunsetUtcHours  = jdToUtcHours(jSet);
    }

    return result;
}

} // namespace heliocelestia::astronomy
