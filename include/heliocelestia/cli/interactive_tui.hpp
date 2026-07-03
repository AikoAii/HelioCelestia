#pragma once

#include <iosfwd>

namespace heliocelestia::cli {

class InteractiveTui {
public:
    int run(std::istream& input, std::ostream& output) const;
};

} // namespace heliocelestia::cli
