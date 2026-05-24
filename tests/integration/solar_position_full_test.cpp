#include <cassert>
#include <cmath>

#include "heliocelestia/services/solar_service.hpp"
#include "heliocelestia/models/observer.hpp"
#include "heliocelestia/core/datetime.hpp"

int main()
{
    using heliocelestia::services::SolarService;
    using heliocelestia::models::Observer;
    using heliocelestia::core::DateTime;

    //
    // NOAA Solar Calculator cross-validation.
    //
    // Reference: https://gml.noaa.gov/grad/solcalc/
    //
    // Observer : London, UK — 51.5°N, −0.12°E
    // Date/Time: 2025-06-21 12:00 UTC (summer solstice, solar noon)
    //
    // NOAA expected values (±0.5° tolerance):
    //   Altitude : ~58.5°
    //   Azimuth  : ~179.0° (nearly due South)
    //

    const Observer london {
        .latitudeDegrees  = 51.5,
        .longitudeDegrees = -0.12,
        .elevationMeters  = 0.0
    };

    const DateTime noonUtc {
        .year                = 2025,
        .month               = 6,
        .day                 = 21,
        .hour                = 12,
        .minute              = 0,
        .second              = 0.0,
        .timezoneOffsetHours = 0.0
    };

    SolarService service;

    const auto pos = service.calculateSolarPosition(london, noonUtc);

    //
    // Altitude tolerance: ±0.5° (simplified solar model vs NOAA).
    //

    assert(std::abs(pos.altitudeDegrees - 58.5) < 0.5);

    //
    // Azimuth: Sun should be within ±2° of due South (180°).
    //

    assert(std::abs(pos.azimuthDegrees - 180.0) < 2.0);

    //
    // Refraction-corrected altitude must be ≥ geometric altitude.
    //

    assert(pos.altitudeCorrectedDegrees >= pos.altitudeDegrees);

    //
    // Declination near solstice: ~+23.4° ± 0.2°
    //

    assert(std::abs(pos.declinationDegrees - 23.4) < 0.2);

    //
    // Julian Date sanity: J2000 is 2451545.0 (2000-01-01 12:00 UTC).
    // 2025 is ~25 years = ~9131 days after J2000.
    //

    assert(pos.julianDate > 2460800.0 && pos.julianDate < 2460900.0);

    //
    // Night-time sanity check:
    // Same location at midnight UTC — Sun must be below horizon.
    //

    const DateTime midnightUtc {
        .year  = 2025, .month = 6, .day = 21,
        .hour  = 0,    .minute = 0
    };

    const auto nightPos = service.calculateSolarPosition(london, midnightUtc);

    assert(nightPos.altitudeDegrees < 0.0);

    return 0;
}
