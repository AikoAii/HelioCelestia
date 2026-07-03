#include <iostream>
#include <optional>

#include "heliocelestia/cli/argument_parser.hpp"

#include "heliocelestia/io/console_renderer.hpp"
#include "heliocelestia/io/json_formatter.hpp"

#include "heliocelestia/services/solar_service.hpp"

#include "heliocelestia/validation/coordinate_validator.hpp"
#include "heliocelestia/validation/datetime_validator.hpp"

int main(int argc, char* argv[])
{
    using namespace heliocelestia;

    models::Observer observer {};
    core::DateTime   dateTime {};

    bool showSunrise = false;
    bool jsonOutput  = false;
    bool verbose     = false;
    bool asciiOutput = false;

    // ─────────────────────────────────────────────────────────
    // CLI mode — all inputs from command-line flags
    // ─────────────────────────────────────────────────────────

    if (argc > 1) {

        cli::ArgumentParser parser;

        const cli::ParsedArguments args =
            parser.parse(argc, argv);

        if (!args.valid) {
            return 1;
        }

        observer    = args.observer;
        dateTime    = args.dateTime;
        showSunrise = args.showSunrise;
        jsonOutput  = args.jsonOutput;
        verbose     = args.verbose;
        asciiOutput = args.asciiOutput;

    }

    // ─────────────────────────────────────────────────────────
    // Interactive mode — guided prompt
    // ─────────────────────────────────────────────────────────

    else {

        std::cout
            << "╔══════════════════════════════════════════╗\n"
            << "║             HelioCelestia                ║\n"
            << "║    Scientific Solar Position Engine      ║\n"
            << "╚══════════════════════════════════════════╝\n\n";

        std::cout << "Observer Coordinates\n";

        std::cout << "  Latitude   (–90 to +90)    : ";
        std::cin  >> observer.latitudeDegrees;

        std::cout << "  Longitude  (–180 to +180)  : ";
        std::cin  >> observer.longitudeDegrees;

        std::cout << "  Elevation  (metres, 0)     : ";
        std::cin  >> observer.elevationMeters;

        std::cout << "\nDate & Time\n";

        std::cout << "  Year      : "; std::cin >> dateTime.year;
        std::cout << "  Month     : "; std::cin >> dateTime.month;
        std::cout << "  Day       : "; std::cin >> dateTime.day;
        std::cout << "  Hour      : "; std::cin >> dateTime.hour;
        std::cout << "  Minute    : "; std::cin >> dateTime.minute;

        std::cout << "  Timezone  (UTC offset, e.g. 7 for UTC+7): ";
        std::cin  >> dateTime.timezoneOffsetHours;

        dateTime.second              = 0.0;
        observer.timezoneOffsetHours = dateTime.timezoneOffsetHours;

        std::cout << "\nOptions\n";
        std::cout << "  Show sunrise/sunset? (1=yes, 0=no): ";
        int srInput = 0;
        std::cin >> srInput;
        showSunrise = (srInput == 1);

        std::cout << "\n";
    }

    // ─────────────────────────────────────────────────────────
    // Validation
    // ─────────────────────────────────────────────────────────

    if (!validation::isValidLatitude(observer.latitudeDegrees)) {
        std::cerr << "Error: Latitude must be in [–90, 90].\n";
        return 1;
    }

    if (!validation::isValidLongitude(observer.longitudeDegrees)) {
        std::cerr << "Error: Longitude must be in [–180, 180].\n";
        return 1;
    }

    if (!validation::isValidElevation(observer.elevationMeters)) {
        std::cerr << "Error: Elevation out of valid Earth surface range.\n";
        return 1;
    }

    if (!validation::isValidDateTime(dateTime)) {
        std::cerr << "Error: Invalid date/time value.\n";
        return 1;
    }

    // ─────────────────────────────────────────────────────────
    // Computation
    // ─────────────────────────────────────────────────────────

    services::SolarService service;

    const models::SolarPosition position =
        service.calculateSolarPosition(observer, dateTime);

    std::optional<models::SunriseResult> sunrise = std::nullopt;

    if (showSunrise) {
        sunrise = service.calculateSunriseSunset(observer, dateTime);
    }

    // ─────────────────────────────────────────────────────────
    // Verbose diagnostic (--verbose)
    // ─────────────────────────────────────────────────────────

    if (verbose) {
        std::cout
            << "[verbose] Julian Date       : " << position.julianDate           << "\n"
            << "[verbose] Declination       : " << position.declinationDegrees   << "°\n"
            << "[verbose] Right Ascension   : " << position.rightAscensionDegrees << "°\n"
            << "[verbose] Geometric Alt     : " << position.altitudeDegrees       << "°\n"
            << "[verbose] Refraction Corr.  : "
            << (position.altitudeCorrectedDegrees - position.altitudeDegrees)
            << "°\n\n";
    }

    // ─────────────────────────────────────────────────────────
    // Output
    // ─────────────────────────────────────────────────────────

    if (jsonOutput) {
        std::cout << io::formatJson(observer, dateTime, position, sunrise);
    } else {
        io::RenderStyle style {};
        if (asciiOutput) {
            style.glyphMode = io::GlyphMode::Ascii;
        }

        io::ConsoleRenderer renderer { style };
        renderer.renderSolarPosition(position, sunrise);
    }

    return 0;
}
