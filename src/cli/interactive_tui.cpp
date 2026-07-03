#include "heliocelestia/cli/interactive_tui.hpp"

#include "heliocelestia/core/datetime.hpp"
#include "heliocelestia/io/formatter.hpp"
#include "heliocelestia/models/lunar_position.hpp"
#include "heliocelestia/models/observer.hpp"
#include "heliocelestia/models/sunrise_result.hpp"
#include "heliocelestia/services/solar_service.hpp"
#include "heliocelestia/validation/coordinate_validator.hpp"
#include "heliocelestia/validation/datetime_validator.hpp"

#include <istream>
#include <optional>
#include <ostream>
#include <string>

namespace heliocelestia::cli {

namespace {

void printBanner(std::ostream& output)
{
    output
        << "╭──────────────────────────────────────────────╮\n"
        << "│              HelioCelestia TUI               │\n"
        << "│        Interactive Astronomy Console         │\n"
        << "╰──────────────────────────────────────────────╯\n\n";
}

void printMenu(std::ostream& output)
{
    output
        << "  1  Solar position\n"
        << "  2  Solar position + sunrise/sunset\n"
        << "  3  Solar position + Moon position\n"
        << "  4  Full sky report\n"
        << "  0  Exit\n\n"
        << "Select: ";
}

template <typename T>
bool readValue(
    std::istream& input,
    std::ostream& output,
    const char* label,
    T& value
)
{
    output << label;
    if (!(input >> value)) {
        output << "\nError: invalid input.\n";
        return false;
    }

    return true;
}

bool readObserverAndDateTime(
    std::istream& input,
    std::ostream& output,
    models::Observer& observer,
    core::DateTime& dateTime
)
{
    output << "\nObserver\n";

    if (!readValue(input, output, "  Latitude  [-90, 90]       : ", observer.latitudeDegrees)) return false;
    if (!readValue(input, output, "  Longitude [-180, 180]     : ", observer.longitudeDegrees)) return false;
    if (!readValue(input, output, "  Elevation [m]             : ", observer.elevationMeters)) return false;

    output << "\nDate & Time\n";

    if (!readValue(input, output, "  Year                      : ", dateTime.year)) return false;
    if (!readValue(input, output, "  Month                     : ", dateTime.month)) return false;
    if (!readValue(input, output, "  Day                       : ", dateTime.day)) return false;
    if (!readValue(input, output, "  Hour                      : ", dateTime.hour)) return false;
    if (!readValue(input, output, "  Minute                    : ", dateTime.minute)) return false;
    if (!readValue(input, output, "  Timezone offset [hours]   : ", dateTime.timezoneOffsetHours)) return false;

    dateTime.second = 0.0;
    observer.timezoneOffsetHours = dateTime.timezoneOffsetHours;

    return true;
}

bool validateInput(
    std::ostream& output,
    const models::Observer& observer,
    const core::DateTime& dateTime
)
{
    if (!validation::isValidLatitude(observer.latitudeDegrees)) {
        output << "\nError: Latitude must be in [-90, 90].\n";
        return false;
    }

    if (!validation::isValidLongitude(observer.longitudeDegrees)) {
        output << "\nError: Longitude must be in [-180, 180].\n";
        return false;
    }

    if (!validation::isValidElevation(observer.elevationMeters)) {
        output << "\nError: Elevation out of valid Earth surface range.\n";
        return false;
    }

    if (!validation::isValidDateTime(dateTime)) {
        output << "\nError: Invalid date/time value.\n";
        return false;
    }

    return true;
}

} // namespace

int InteractiveTui::run(std::istream& input, std::ostream& output) const
{
    printBanner(output);

    while (true) {
        printMenu(output);

        int choice = 0;
        if (!(input >> choice)) {
            output << "\nError: invalid menu selection.\n";
            return 1;
        }

        if (choice == 0) {
            output << "\nGoodbye.\n";
            return 0;
        }

        if (choice < 0 || choice > 4) {
            output << "\nError: unknown menu selection.\n\n";
            continue;
        }

        models::Observer observer {};
        core::DateTime dateTime {};

        if (!readObserverAndDateTime(input, output, observer, dateTime)) {
            return 1;
        }

        if (!validateInput(output, observer, dateTime)) {
            return 1;
        }

        const bool includeSunrise = (choice == 2 || choice == 4);
        const bool includeMoon = (choice == 3 || choice == 4);

        services::SolarService service;

        const models::SolarPosition solarPosition =
            service.calculateSolarPosition(observer, dateTime);

        std::optional<models::SunriseResult> sunrise = std::nullopt;
        if (includeSunrise) {
            sunrise = service.calculateSunriseSunset(observer, dateTime);
        }

        std::optional<models::LunarPosition> moon = std::nullopt;
        if (includeMoon) {
            moon = service.calculateLunarPosition(observer, dateTime);
        }

        output << "\n";
        io::ConsoleRenderer renderer;
        output << io::formatSolarPosition(solarPosition, sunrise, {}, moon);
        output << "\n";
    }
}

} // namespace heliocelestia::cli
