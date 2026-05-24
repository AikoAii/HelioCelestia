#include "heliocelestia/io/json_formatter.hpp"

#include <iomanip>
#include <sstream>
#include <cmath>

namespace heliocelestia::io {

namespace {

//
// Minimal JSON helpers — no external dependency.
//

std::string jsonReal(const double value, const int precision = 4)
{
    std::ostringstream ss;
    ss << std::fixed << std::setprecision(precision) << value;
    return ss.str();
}

std::string jsonBool(const bool value)
{
    return value ? "true" : "false";
}

//
// Format a fractional-hours value as "HH:MM UTC".
//

std::string formatUtcTime(const double fractionalHours)
{
    const int hours   = static_cast<int>(fractionalHours) % 24;
    const int minutes = static_cast<int>(
        std::round((fractionalHours - std::floor(fractionalHours)) * 60.0)
    ) % 60;

    std::ostringstream ss;
    ss << std::setfill('0')
       << std::setw(2) << hours
       << ":"
       << std::setw(2) << minutes
       << " UTC";

    return ss.str();
}

//
// Derive a human-readable sky status from the apparent altitude.
//

std::string skyStatus(const double altitudeCorrectedDegrees)
{
    if (altitudeCorrectedDegrees >  0.0)  return "visible";
    if (altitudeCorrectedDegrees > -6.0)  return "civil_twilight";
    if (altitudeCorrectedDegrees > -12.0) return "nautical_twilight";
    if (altitudeCorrectedDegrees > -18.0) return "astronomical_twilight";
    return "night";
}

} // anonymous namespace

std::string formatJson(
    const models::Observer&                     observer,
    const core::DateTime&                       dateTime,
    const models::SolarPosition&                position,
    const std::optional<models::SunriseResult>& sunrise
)
{
    std::ostringstream out;

    out << "{\n";

    //
    // Observer
    //

    out << "  \"observer\": {\n";
    out << "    \"latitude_deg\":  " << jsonReal(observer.latitudeDegrees)  << ",\n";
    out << "    \"longitude_deg\": " << jsonReal(observer.longitudeDegrees) << ",\n";
    out << "    \"elevation_m\":   " << jsonReal(observer.elevationMeters, 1) << "\n";
    out << "  },\n";

    //
    // Date/time
    //

    out << "  \"datetime\": {\n";
    out << "    \"year\":     " << dateTime.year   << ",\n";
    out << "    \"month\":    " << dateTime.month  << ",\n";
    out << "    \"day\":      " << dateTime.day    << ",\n";
    out << "    \"hour\":     " << dateTime.hour   << ",\n";
    out << "    \"minute\":   " << dateTime.minute << ",\n";
    out << "    \"second\":   " << jsonReal(dateTime.second, 1) << ",\n";
    out << "    \"timezone\": " << jsonReal(dateTime.timezoneOffsetHours, 1) << "\n";
    out << "  },\n";

    //
    // Solar position
    //

    out << "  \"solar_position\": {\n";
    out << "    \"altitude_deg\":           " << jsonReal(position.altitudeDegrees)          << ",\n";
    out << "    \"altitude_corrected_deg\": " << jsonReal(position.altitudeCorrectedDegrees) << ",\n";
    out << "    \"azimuth_deg\":            " << jsonReal(position.azimuthDegrees)           << ",\n";
    out << "    \"declination_deg\":        " << jsonReal(position.declinationDegrees)       << ",\n";
    out << "    \"right_ascension_deg\":    " << jsonReal(position.rightAscensionDegrees)    << ",\n";
    out << "    \"julian_date\":            " << jsonReal(position.julianDate, 5)            << ",\n";
    out << "    \"status\":                 \"" << skyStatus(position.altitudeCorrectedDegrees) << "\"\n";
    out << "  }";

    //
    // Sunrise / sunset (optional section)
    //

    if (sunrise.has_value()) {

        const auto& sr = *sunrise;

        out << ",\n";
        out << "  \"sunrise_sunset\": {\n";
        out << "    \"polar_day\":   " << jsonBool(sr.isPolarDay)   << ",\n";
        out << "    \"polar_night\": " << jsonBool(sr.isPolarNight) << ",\n";
        out << "    \"solar_noon\":  \"" << formatUtcTime(sr.solarNoonUtcHours) << "\",\n";

        if (sr.sunriseUtcHours.has_value()) {
            out << "    \"sunrise\":     \"" << formatUtcTime(*sr.sunriseUtcHours) << "\",\n";
        } else {
            out << "    \"sunrise\":     null,\n";
        }

        if (sr.sunsetUtcHours.has_value()) {
            out << "    \"sunset\":      \"" << formatUtcTime(*sr.sunsetUtcHours) << "\"\n";
        } else {
            out << "    \"sunset\":      null\n";
        }

        out << "  }";
    }

    out << "\n}\n";

    return out.str();
}

} // namespace heliocelestia::io
