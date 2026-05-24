#include "heliocelestia/services/solar_service.hpp"

#include "heliocelestia/astronomy/coordinate_transform.hpp"
#include "heliocelestia/astronomy/julian.hpp"
#include "heliocelestia/astronomy/refraction.hpp"
#include "heliocelestia/astronomy/sidereal.hpp"
#include "heliocelestia/astronomy/solar_model.hpp"
#include "heliocelestia/astronomy/sunrise_sunset.hpp"

namespace heliocelestia::services {

models::SolarPosition
SolarService::calculateSolarPosition(
    const models::Observer& observer,
    const core::DateTime&   dateTime
) const noexcept
{

    const core::Real julianDate =
        astronomy::calculateJulianDate(dateTime);

    const core::Real greenwichSiderealTime =
        astronomy::calculateGreenwichSiderealTime(julianDate);

    const core::Real localSiderealTime =
        astronomy::calculateLocalSiderealTime(
            greenwichSiderealTime,
            observer.longitudeDegrees
        );


    const models::EquatorialCoordinate equatorial =
        astronomy::calculateSolarEquatorialCoordinate(dateTime);

    //
    // Step 5: Project celestial coordinates onto the observer's local sky.
    //

    const models::HorizontalCoordinate horizontal =
        astronomy::transformEquatorialToHorizontal(
            equatorial,
            localSiderealTime,
            observer
        );

    //
    // Step 6: Apply atmospheric refraction correction.
    //
    // The raw altitude is the geometric position.
    // The corrected altitude is what an observer physically sees.
    //

    const core::Real refractionDegrees =
        astronomy::calculateAtmosphericRefraction(
            horizontal.altitudeDegrees
        );

    return {
        .altitudeDegrees          = horizontal.altitudeDegrees,
        .altitudeCorrectedDegrees = horizontal.altitudeDegrees
                                    + refractionDegrees,
        .azimuthDegrees           = horizontal.azimuthDegrees,
        .declinationDegrees       = equatorial.declinationDegrees,
        .rightAscensionDegrees    = equatorial.rightAscensionDegrees,
        .julianDate               = julianDate
    };
}

models::SunriseResult
SolarService::calculateSunriseSunset(
    const models::Observer& observer,
    const core::DateTime&   date
) const noexcept
{
    return astronomy::calculateSunriseSunset(observer, date);
}

} // namespace heliocelestia::services