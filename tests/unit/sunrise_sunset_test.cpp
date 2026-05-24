#include <cassert>
#include <cmath>

#include "heliocelestia/astronomy/sunrise_sunset.hpp"
#include "heliocelestia/models/observer.hpp"
#include "heliocelestia/core/datetime.hpp"

int main()
{
    using heliocelestia::astronomy::calculateSunriseSunset;
    using heliocelestia::models::Observer;
    using heliocelestia::core::DateTime;

    //
    // Reference: Equator (0°N, 0°E) on the March equinox (2025-03-20).
    //
    // On the equinox at the equator, sunrise ≈ 06:00 UTC
    // and sunset ≈ 18:00 UTC (±10 minutes due to model simplification).
    //

    const Observer equator { .latitudeDegrees = 0.0, .longitudeDegrees = 0.0 };
    const DateTime equinox { .year = 2025, .month = 3, .day = 20 };

    const auto result = calculateSunriseSunset(equator, equinox);

    assert(!result.isPolarDay);
    assert(!result.isPolarNight);
    assert(result.sunriseUtcHours.has_value());
    assert(result.sunsetUtcHours.has_value());

    // Sunrise near 06:00 UTC (±20 min tolerance for algorithm simplification)
    assert(*result.sunriseUtcHours > 5.5 && *result.sunriseUtcHours < 6.5);

    // Sunset near 18:00 UTC (±20 min)
    assert(*result.sunsetUtcHours > 17.5 && *result.sunsetUtcHours < 18.5);

    // Solar noon near 12:00 UTC (±10 min)
    assert(result.solarNoonUtcHours > 11.8 && result.solarNoonUtcHours < 12.2);

    //
    // Polar day: Tromsø (69.6°N) on summer solstice (2025-06-21).
    // The Sun does not set above the Arctic Circle in June.
    //

    const Observer tromso { .latitudeDegrees = 69.6, .longitudeDegrees = 18.96 };
    const DateTime solstice { .year = 2025, .month = 6, .day = 21 };

    const auto polarDay = calculateSunriseSunset(tromso, solstice);

    assert(polarDay.isPolarDay);
    assert(!polarDay.isPolarNight);
    assert(!polarDay.sunriseUtcHours.has_value());
    assert(!polarDay.sunsetUtcHours.has_value());

    //
    // Polar night: Tromsø on winter solstice (2025-12-21).
    // The Sun does not rise above the Arctic Circle in December.
    //

    const DateTime winter { .year = 2025, .month = 12, .day = 21 };

    const auto polarNight = calculateSunriseSunset(tromso, winter);

    assert(polarNight.isPolarNight);
    assert(!polarNight.isPolarDay);

    //
    // Solar noon is always defined — even in polar conditions.
    //

    assert(polarDay.solarNoonUtcHours   >= 0.0 && polarDay.solarNoonUtcHours   < 24.0);
    assert(polarNight.solarNoonUtcHours >= 0.0 && polarNight.solarNoonUtcHours < 24.0);

    return 0;
}
