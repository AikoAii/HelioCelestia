#include "heliocelestia/validation/coordinate_validator.hpp"

namespace heliocelestia::validation {

bool isValidLatitude(
    const core::Real latitudeDegrees
) noexcept
{
    return
        latitudeDegrees >= -90.0
        &&
        latitudeDegrees <= 90.0;
}

bool isValidLongitude(
    const core::Real longitudeDegrees
) noexcept
{
    return
        longitudeDegrees >= -180.0
        &&
        longitudeDegrees <= 180.0;
}

bool isValidElevation(
    const core::Real elevationMeters
) noexcept
{
    //
    // Lowest surface point: Dead Sea (~−430 m).
    // Highest surface point: Everest (~+8849 m).
    // We add ±500 m headroom for corner-cases near the extremes.
    //

    return
        elevationMeters >= -930.0
        &&
        elevationMeters <= 9350.0;
}

} // namespace heliocelestia::validation