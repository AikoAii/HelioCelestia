#include "heliocelestia/math/angle.hpp"

#include <cmath>

namespace heliocelestia::math {

core::Real normalizeDegrees(core::Real degrees) noexcept {
  //
  // Normalize angle into:
  //
  // [0°, 360°)
  //
  // Examples:
  //
  // 370°  -> 10°
  // -45°  -> 315°
  // 720°  -> 0°
  //
  // std::fmod is used because:
  // - '%' only works for integers
  // - astronomy uses floating-point angles
  //

  degrees = std::fmod(degrees, core::kDegreesPerCircle);

  //
  // fmod keeps negative signs:
  //
  // fmod(-45, 360) -> -45
  //
  // Convert negative angles into positive cyclic range.
  //

  if (degrees < 0.0) {
    degrees += core::kDegreesPerCircle;
  }

  return degrees;
}

core::Real normalizeRadians(core::Real radians) noexcept {
  //
  // Normalize angle into:
  //
  // [0, 2π)
  //

  radians = std::fmod(radians, core::kTwoPi);

  if (radians < 0.0) {
    radians += core::kTwoPi;
  }

  return radians;
}

} // namespace heliocelestia::math
