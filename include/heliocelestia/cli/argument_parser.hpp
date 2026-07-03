#pragma once

#include "heliocelestia/core/datetime.hpp"
#include "heliocelestia/models/observer.hpp"

namespace heliocelestia::cli {

struct ParsedArguments {
    models::Observer observer {};
    core::DateTime dateTime {};
    bool showSunrise { false };
    bool jsonOutput { false };
    bool verbose { false };
    bool asciiOutput { false };
    bool valid { false };
};

class ArgumentParser {
public:
    ParsedArguments parse(int argc, char* argv[]) const;
    void printHelp() const;
};

} // namespace heliocelestia::cli
