#include "heliocelestia/astronomy/refraction.hpp"

#include "heliocelestia/math/angle.hpp"

#include <cmath>

namespace heliocelestia::astronomy {

core::Real calculateAtmosphericRefraction(
    const core::Real geometricAltitudeDegrees
) noexcept
{
    //
    // Bennett (1982) atmospheric refraction formula.
    //
    // For altitudes below −0.5°, the Sun is well below the
    // horizon and refraction plays no role in the displayed result.
    //

    if (geometricAltitudeDegrees < -0.5) {
        return 0.0;
    }

    const core::Real h = geometricAltitudeDegrees;

    //
    // The argument of the tangent is a polynomial in h that
    // gives very accurate results for h ≥ 5° and acceptable
    // results down to the horizon.
    //
    // Result R is in arcminutes.
    //

    const core::Real argumentDegrees =
        h + (10.3 / (h + 5.11));

    const core::Real argumentRadians =
        math::degToRad(argumentDegrees);

    const core::Real refractionArcminutes =
        1.02 / std::tan(argumentRadians);

    //
    // Convert arcminutes → degrees.
    //

    return refractionArcminutes / 60.0;
}

} // namespace heliocelestia::astronomy
