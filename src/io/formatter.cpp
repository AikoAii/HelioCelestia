#include "heliocelestia/io/formatter.hpp"

#include "heliocelestia/io/key_value_renderer.hpp"
#include "heliocelestia/io/section_renderer.hpp"

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

std::string formatFixed(const double value, const int precision = 4)
{
    std::ostringstream ss;
    ss << std::fixed << std::setprecision(precision) << value;
    return ss.str();
}

} // anonymous namespace

std::string formatSolarPosition(
    const models::SolarPosition&                position,
    const std::optional<models::SunriseResult>& sunrise,
    RenderStyle                                 style,
    const std::optional<models::LunarPosition>& moon
)
{
    std::ostringstream out;

    const SectionRenderer sections { style };
    const KeyValueRenderer kv { style };

    out << sections.banner(
        "HelioCelestia Report",
        "Scientific Solar Position Engine"
    );

    //
    // Solar position
    //

    out << sections.section("Solar Position");

    out << kv.render("Altitude (geometric)", formatFixed(position.altitudeDegrees), "deg");
    out << kv.render(
        "Altitude (apparent)",
        formatFixed(position.altitudeCorrectedDegrees),
        "deg",
        "(refraction corrected)"
    );
    out << kv.render("Azimuth", formatFixed(position.azimuthDegrees), "deg");
    out << kv.render("Declination", formatFixed(position.declinationDegrees), "deg");
    out << kv.render("Right Ascension", formatFixed(position.rightAscensionDegrees), "deg");
    out << kv.render("Julian Date", formatFixed(position.julianDate, 5));

    out << "\n";

    out << kv.render(
        "Status",
        skyStatusLabel(position.altitudeCorrectedDegrees)
    );

    //
    // Sunrise / sunset section (shown only when requested)
    //

    if (sunrise.has_value()) {

        const auto& sr = *sunrise;

        out << "\n";
        out << sections.section("Day Events");

        if (sr.isPolarDay) {
            out << kv.render(
                "Polar Day",
                "Sun remains above the horizon all day."
            );
        } else if (sr.isPolarNight) {
            out << kv.render(
                "Polar Night",
                "Sun remains below the horizon all day."
            );
        } else {
            if (sr.sunriseUtcHours.has_value()) {
                out << kv.render("Sunrise", formatUtcTime(*sr.sunriseUtcHours));
            }

            out << kv.render("Solar Noon", formatUtcTime(sr.solarNoonUtcHours));

            if (sr.sunsetUtcHours.has_value()) {
                out << kv.render("Sunset", formatUtcTime(*sr.sunsetUtcHours));
            }
        }
    }

    if (moon.has_value()) {
        out << "\n";
        out << sections.section("Moon Position");

        out << kv.render("Altitude", formatFixed(moon->altitudeDegrees), "deg");
        out << kv.render("Azimuth", formatFixed(moon->azimuthDegrees), "deg");
        out << kv.render("Declination", formatFixed(moon->declinationDegrees), "deg");
        out << kv.render("Right Ascension", formatFixed(moon->rightAscensionDegrees), "deg");
        out << kv.render("Ecliptic Longitude", formatFixed(moon->eclipticLongitudeDegrees), "deg");
        out << kv.render("Ecliptic Latitude", formatFixed(moon->eclipticLatitudeDegrees), "deg");
        out << kv.render("Distance", formatFixed(moon->distanceKilometers, 0), "km");
    }

    out << "\n";

    return out.str();
}

} // namespace heliocelestia::io
