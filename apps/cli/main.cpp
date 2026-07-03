#include <iostream>
#include <optional>

#include "heliocelestia/cli/argument_parser.hpp"
#include "heliocelestia/cli/interactive_tui.hpp"

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
    bool showMoon    = false;
    bool tuiMode     = false;

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
        showMoon    = args.showMoon;
        tuiMode     = args.tuiMode;

    }

    // ─────────────────────────────────────────────────────────
    // Interactive mode — guided prompt
    // ─────────────────────────────────────────────────────────

    else {
        cli::InteractiveTui tui;
        return tui.run(std::cin, std::cout);
    }

    if (tuiMode) {
        cli::InteractiveTui tui;
        return tui.run(std::cin, std::cout);
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
    std::optional<models::LunarPosition> moon = std::nullopt;

    if (showSunrise) {
        sunrise = service.calculateSunriseSunset(observer, dateTime);
    }

    if (showMoon) {
        moon = service.calculateLunarPosition(observer, dateTime);
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
        std::cout << io::formatJson(observer, dateTime, position, sunrise, moon);
    } else {
        io::RenderStyle style {};
        if (asciiOutput) {
            style.glyphMode = io::GlyphMode::Ascii;
        }

        io::ConsoleRenderer renderer { style };
        renderer.renderSolarPosition(position, sunrise, moon);
    }

    return 0;
}
