#include "heliocelestia/cli/argument_parser.hpp"

#include <iostream>
#include <string>

namespace heliocelestia::cli {

namespace {

//
// Safe string-to-double conversion.
// Returns false and leaves `out` unchanged if parsing fails.
//

bool parseDouble(const char* str, double& out)
{
    try {
        std::size_t pos = 0;
        out = std::stod(str, &pos);
        return pos > 0;
    } catch (const std::exception&) {
        return false;
    }
}

bool parseInt(const char* str, int& out)
{
    try {
        std::size_t pos = 0;
        out = std::stoi(str, &pos);
        return pos > 0;
    } catch (const std::exception&) {
        return false;
    }
}

} // anonymous namespace

ParsedArguments ArgumentParser::parse(
    int    argc,
    char*  argv[]
) const
{
    ParsedArguments args {};

    for (int i = 1; i < argc; ++i) {

        const std::string flag = argv[i];

        //
        // Boolean flags (no value argument).
        //

        if (flag == "--help" || flag == "-h") {
            printHelp();
            return args; // valid == false → caller exits cleanly
        }

        if (flag == "--json") {
            args.jsonOutput = true;
            continue;
        }

        if (flag == "--sunrise") {
            args.showSunrise = true;
            continue;
        }

        if (flag == "--verbose") {
            args.verbose = true;
            continue;
        }

        //
        // Value flags — require the next token.
        //

        if (i + 1 >= argc) {
            std::cerr
                << "Error: '" << flag
                << "' requires a value.\n";
            return args;
        }

        const char* value = argv[++i];

        if (flag == "--latitude") {
            if (!parseDouble(value, args.observer.latitudeDegrees)) {
                std::cerr << "Error: invalid latitude '" << value << "'.\n";
                return args;
            }
        }

        else if (flag == "--longitude") {
            if (!parseDouble(value, args.observer.longitudeDegrees)) {
                std::cerr << "Error: invalid longitude '" << value << "'.\n";
                return args;
            }
        }

        else if (flag == "--elevation") {
            if (!parseDouble(value, args.observer.elevationMeters)) {
                std::cerr << "Error: invalid elevation '" << value << "'.\n";
                return args;
            }
        }

        else if (flag == "--year") {
            if (!parseInt(value, args.dateTime.year)) {
                std::cerr << "Error: invalid year '" << value << "'.\n";
                return args;
            }
        }

        else if (flag == "--month") {
            if (!parseInt(value, args.dateTime.month)) {
                std::cerr << "Error: invalid month '" << value << "'.\n";
                return args;
            }
        }

        else if (flag == "--day") {
            if (!parseInt(value, args.dateTime.day)) {
                std::cerr << "Error: invalid day '" << value << "'.\n";
                return args;
            }
        }

        else if (flag == "--hour") {
            if (!parseInt(value, args.dateTime.hour)) {
                std::cerr << "Error: invalid hour '" << value << "'.\n";
                return args;
            }
        }

        else if (flag == "--minute") {
            if (!parseInt(value, args.dateTime.minute)) {
                std::cerr << "Error: invalid minute '" << value << "'.\n";
                return args;
            }
        }

        else if (flag == "--second") {
            if (!parseDouble(value, args.dateTime.second)) {
                std::cerr << "Error: invalid second '" << value << "'.\n";
                return args;
            }
        }

        else if (flag == "--timezone") {
            if (!parseDouble(value, args.dateTime.timezoneOffsetHours)) {
                std::cerr << "Error: invalid timezone '" << value << "'.\n";
                return args;
            }
            args.observer.timezoneOffsetHours =
                args.dateTime.timezoneOffsetHours;
        }

        else {
            std::cerr
                << "Error: unknown flag '" << flag
                << "'. Run --help for usage.\n";
            return args;
        }
    }

    args.valid = true;
    return args;
}

void ArgumentParser::printHelp() const
{
    std::cout
        << "HelioCelestia — Scientific Solar Position Engine\n"
        << "Version 0.2.0  |  C++20  |  CMake\n\n"
        << "Usage:\n"
        << "  heliocelestia_cli [options]\n\n"
        << "Observer:\n"
        << "  --latitude   <deg>    Geographic latitude   [-90, 90]\n"
        << "  --longitude  <deg>    Geographic longitude  [-180, 180]\n"
        << "  --elevation  <m>      Observer elevation in metres (default: 0)\n\n"
        << "Date & Time:\n"
        << "  --year       <int>    Year  (e.g. 2025)\n"
        << "  --month      <int>    Month [1, 12]\n"
        << "  --day        <int>    Day   [1, 31]\n"
        << "  --hour       <int>    Hour  [0, 23]\n"
        << "  --minute     <int>    Minute [0, 59]\n"
        << "  --second     <real>   Second [0.0, 60.0] (default: 0)\n"
        << "  --timezone   <real>   UTC offset in hours, e.g. 7 for UTC+7\n\n"
        << "Output:\n"
        << "  --json                Print result as JSON\n"
        << "  --sunrise             Include sunrise / solar noon / sunset\n"
        << "  --verbose             Show additional diagnostic information\n\n"
        << "Help:\n"
        << "  --help, -h            Show this message\n\n"
        << "Examples:\n"
        << "  # Solar position in London at noon BST:\n"
        << "  heliocelestia_cli --latitude 51.5 --longitude -0.12 \\\n"
        << "                    --year 2025 --month 6 --day 21 \\\n"
        << "                    --hour 12 --minute 0 --timezone 1\n\n"
        << "  # JSON output with sunrise times:\n"
        << "  heliocelestia_cli --latitude 51.5 --longitude -0.12 \\\n"
        << "                    --year 2025 --month 6 --day 21 \\\n"
        << "                    --hour 12 --minute 0 --timezone 1 \\\n"
        << "                    --json --sunrise\n\n"
        << "  # Interactive mode (no arguments):\n"
        << "  heliocelestia_cli\n\n"
        << "Run without arguments for the interactive prompt.\n";
}

} // namespace heliocelestia::cli