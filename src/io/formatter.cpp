#include "heliocelestia/io/formatter.hpp"

#include <cmath>
#include <iomanip>
#include <sstream>

namespace heliocelestia::io {

namespace {

//
// Classify apparent altitude into the standard twilight zones.
//

std::string skyStatusLabel(const double alt)
{
    if (alt >  0.0)  return "Sun Visible";
    if (alt > -6.0)  return "Civil Twilight";
    if (alt > -12.0) return "Nautical Twilight";
    if (alt > -18.0) return "Astronomical Twilight";
    return "Night";
}

//
// Format fractional UTC hours as "HH:MM UTC".
//

std::string formatUtcTime(const double fractionalHours)
{
    const int h = static_cast<int>(fractionalHours) % 24;
    const int m = static_cast<int>(
        std::round((fractionalHours - std::floor(fractionalHours)) * 60.0)
    ) % 60;

    std::ostringstream ss;
    ss << std::setfill('0') << std::setw(2) << h
       << ":" << std::setw(2) << m << " UTC";

    return ss.str();
}

} // anonymous namespace

std::string formatSolarPosition(
    const models::SolarPosition&                position,
    const std::optional<models::SunriseResult>& sunrise
)
{
    std::ostringstream out;

    out << std::fixed << std::setprecision(4);

    out << "╔══════════════════════════════════════════╗\n";
    out << "║           HelioCelestia Report           ║\n";
    out << "╚══════════════════════════════════════════╝\n\n";

    //
    // Solar position
    //

    out << "  Solar Position\n";
    out << "  ─────────────────────────────────────────\n";

    out << "  Altitude (geometric)   : "
        << std::setw(10) << position.altitudeDegrees
        << "°\n";

    out << "  Altitude (apparent)    : "
        << std::setw(10) << position.altitudeCorrectedDegrees
        << "°  (refraction corrected)\n";

    out << "  Azimuth                : "
        << std::setw(10) << position.azimuthDegrees
        << "°\n";

    out << "  Declination            : "
        << std::setw(10) << position.declinationDegrees
        << "°\n";

    out << "  Right Ascension        : "
        << std::setw(10) << position.rightAscensionDegrees
        << "°\n";

    out << "  Julian Date            : "
        << std::setprecision(5)
        << position.julianDate
        << "\n";

    out << "\n";

    out << "  Status  : "
        << skyStatusLabel(position.altitudeCorrectedDegrees)
        << "\n";

    //
    // Sunrise / sunset section (shown only when requested)
    //

    if (sunrise.has_value()) {

        const auto& sr = *sunrise;

        out << "\n";
        out << "  Day Events\n";
        out << "  ─────────────────────────────────────────\n";

        if (sr.isPolarDay) {
            out << "  Polar Day  : Sun remains above the horizon all day.\n";
        } else if (sr.isPolarNight) {
            out << "  Polar Night: Sun remains below the horizon all day.\n";
        } else {
            if (sr.sunriseUtcHours.has_value()) {
                out << "  Sunrise    : " << formatUtcTime(*sr.sunriseUtcHours) << "\n";
            }

            out << "  Solar Noon : " << formatUtcTime(sr.solarNoonUtcHours) << "\n";

            if (sr.sunsetUtcHours.has_value()) {
                out << "  Sunset     : " << formatUtcTime(*sr.sunsetUtcHours) << "\n";
            }
        }
    }

    out << "\n";

    return out.str();
}

} // namespace heliocelestia::io