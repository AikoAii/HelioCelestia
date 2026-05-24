#include "heliocelestia/math/angle.hpp"

#include <cassert>

int main()
{
    using namespace heliocelestia;

    //
    // Degree normalization tests.
    //

    assert(
        math::normalizeDegrees(370.0)
        ==
        10.0
    );

    assert(
        math::normalizeDegrees(-45.0)
        ==
        315.0
    );

    assert(
        math::normalizeDegrees(720.0)
        ==
        0.0
    );

    //
    // Degree ↔ radian conversion tests.
    //

    constexpr core::Real pi =
        core::kPi;

    assert(
        math::degToRad(180.0)
        ==
        pi
    );

    assert(
        math::radToDeg(pi)
        ==
        180.0
    );

    return 0;
}