#include "heliocelestia/io/formatter.hpp"

#include <cassert>
#include <optional>
#include <string>

int main()
{
    using namespace heliocelestia;

    const models::SolarPosition position {
        .altitudeDegrees = 62.34,
        .altitudeCorrectedDegrees = 62.35,
        .azimuthDegrees = 281.45,
        .declinationDegrees = 23.4,
        .rightAscensionDegrees = 91.2,
        .julianDate = 2460848.0
    };

    const models::LunarPosition moon {
        .altitudeDegrees = 12.3,
        .azimuthDegrees = 210.4,
        .declinationDegrees = -12.5,
        .rightAscensionDegrees = 330.2,
        .eclipticLongitudeDegrees = 315.0,
        .eclipticLatitudeDegrees = 2.1,
        .distanceKilometers = 380000.0,
        .julianDate = 2460848.0
    };

    const std::string unicodeOutput =
        io::formatSolarPosition(position, std::nullopt);

    assert(unicodeOutput.find("HelioCelestia Report") != std::string::npos);
    assert(unicodeOutput.find("Solar Position") != std::string::npos);
    assert(unicodeOutput.find("Altitude (geometric)") != std::string::npos);
    assert(unicodeOutput.find("╭") != std::string::npos);

    const std::string moonOutput =
        io::formatSolarPosition(position, std::nullopt, {}, moon);

    assert(moonOutput.find("Moon Position") != std::string::npos);
    assert(moonOutput.find("Distance") != std::string::npos);

    io::RenderStyle asciiStyle {};
    asciiStyle.glyphMode = io::GlyphMode::Ascii;

    const std::string asciiOutput =
        io::formatSolarPosition(position, std::nullopt, asciiStyle);

    assert(asciiOutput.find("+") != std::string::npos);
    assert(asciiOutput.find("-") != std::string::npos);
    assert(asciiOutput.find("╭") == std::string::npos);

    return 0;
}
