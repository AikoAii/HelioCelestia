#include "heliocelestia/astronomy/sidereal.hpp"

#include <cassert>

int main()
{
    using namespace heliocelestia;

    //
    // Reference Julian Date:
    // J2000 epoch
    //

    constexpr core::Real julianDate =
        2451545.0;

    const core::Real gst =
        astronomy::
        calculateGreenwichSiderealTime(
            julianDate
        );

    //
    // GST should normalize
    // into cyclic degree range.
    //

    assert(gst >= 0.0);

    assert(gst < 360.0);

    return 0;
}