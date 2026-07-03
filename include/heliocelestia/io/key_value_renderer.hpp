#pragma once

#include "heliocelestia/io/render_style.hpp"

#include <string>
#include <string_view>

namespace heliocelestia::io {

class KeyValueRenderer {
public:
    explicit KeyValueRenderer(RenderStyle style);

    [[nodiscard]] std::string render(
        std::string_view key,
        std::string_view value,
        std::string_view unit = {},
        std::string_view note = {}
    ) const;

private:
    RenderStyle style_;
};

} // namespace heliocelestia::io
