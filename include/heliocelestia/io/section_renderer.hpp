#pragma once

#include "heliocelestia/io/render_style.hpp"

#include <string>
#include <string_view>

namespace heliocelestia::io {

class SectionRenderer {
public:
    explicit SectionRenderer(RenderStyle style);

    [[nodiscard]] std::string banner(
        std::string_view title,
        std::string_view subtitle
    ) const;

    [[nodiscard]] std::string section(std::string_view title) const;
    [[nodiscard]] std::string divider() const;

private:
    RenderStyle style_;
};

} // namespace heliocelestia::io
