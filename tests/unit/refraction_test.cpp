#include <cassert>
#include <cmath>

#include "heliocelestia/astronomy/refraction.hpp"

int main()
{
    using heliocelestia::astronomy::calculateAtmosphericRefraction;

    //
    // Below horizon cutoff — no refraction applied.
    //

    assert(calculateAtmosphericRefraction(-1.0) == 0.0);
    assert(calculateAtmosphericRefraction(-90.0) == 0.0);

    //
    // At the geometric horizon (h = 0°) refraction should be
    // roughly 28–30 arcminutes ≈ 0.47–0.50 degrees.
    // Bennett 1982 gives ~29', i.e. ~0.483°.
    //

    const double R_horizon = calculateAtmosphericRefraction(0.0);
    assert(R_horizon > 0.45 && R_horizon < 0.52);

    //
    // At h = 10°, standard tables give ~5.3 arcminutes ≈ 0.088°.
    // Bennett gives approximately 5.4' ≈ 0.090°.
    //

    const double R_10 = calculateAtmosphericRefraction(10.0);
    assert(R_10 > 0.082 && R_10 < 0.096);

    //
    // At h = 45° refraction is approximately 1 arcminute ≈ 0.0167°.
    //

    const double R_45 = calculateAtmosphericRefraction(45.0);
    assert(R_45 > 0.012 && R_45 < 0.022);

    //
    // At zenith (h = 90°) refraction approaches 0 — tan(90°) → ∞.
    // Practically it should be well below 0.01°.
    //

    const double R_90 = calculateAtmosphericRefraction(90.0);
    assert(R_90 >= 0.0 && R_90 < 0.005);

    //
    // Refraction is monotonically decreasing with altitude.
    //

    assert(calculateAtmosphericRefraction(1.0) > calculateAtmosphericRefraction(5.0));
    assert(calculateAtmosphericRefraction(5.0) > calculateAtmosphericRefraction(30.0));
    assert(calculateAtmosphericRefraction(30.0) > calculateAtmosphericRefraction(80.0));

    return 0;
}
