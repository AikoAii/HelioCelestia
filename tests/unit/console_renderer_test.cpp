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

    const std::string unicodeOutput =
        io::formatSolarPosition(position, std::nullopt);

    assert(unicodeOutput.find("HelioCelestia Report") != std::string::npos);
    assert(unicodeOutput.find("Solar Position") != std::string::npos);
    assert(unicodeOutput.find("Altitude (geometric)") != std::string::npos);
    assert(unicodeOutput.find("╭") != std::string::npos);

    io::RenderStyle asciiStyle {};
    asciiStyle.glyphMode = io::GlyphMode::Ascii;

    const std::string asciiOutput =
        io::formatSolarPosition(position, std::nullopt, asciiStyle);

    assert(asciiOutput.find("+") != std::string::npos);
    assert(asciiOutput.find("-") != std::string::npos);
    assert(asciiOutput.find("╭") == std::string::npos);

    return 0;
}
