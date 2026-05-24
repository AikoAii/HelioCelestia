#include "heliocelestia/astronomy/sidereal.hpp"

#include "heliocelestia/math/angle.hpp"

namespace heliocelestia::astronomy {

core::Real calculateGreenwichSiderealTime(
    core::Real julianDate
) noexcept
{
    //
    // Source:
    // Jean Meeus
    // Astronomical Algorithms
    // Chapter 12
    //
    // Greenwich Sidereal Time (GST)
    // represents Earth's rotation relative
    // to distant stars.
    //

    //
    // Julian centuries since J2000 epoch.
    //

    const core::Real t =
        (julianDate - 2451545.0)
        /
        36525.0;

    //
    // Meeus GST approximation formula.
    //
    // Result is initially in degrees.
    //

    const core::Real gstDegrees =
        280.46061837
        +
        360.98564736629
        *
        (julianDate - 2451545.0)
        +
        0.000387933
        *
        (t * t)
        -
        (
            (t * t * t)
            /
            38710000.0
        );

    //
    // Sidereal angles are cyclic.
    //
    // Normalize into:
    //
    // [0°, 360°)
    //

    return math::normalizeDegrees(
        gstDegrees
    );
}

core::Real calculateLocalSiderealTime(
    core::Real greenwichSiderealTimeDegrees,

    core::Real longitudeDegrees
) noexcept
{
    //
    // Local Sidereal Time (LST)
    // adjusts GST using observer longitude.
    //
    // East longitudes are positive.
    // West longitudes are negative.
    //

    return math::normalizeDegrees(
        greenwichSiderealTimeDegrees
        +
        longitudeDegrees
    );
}

}