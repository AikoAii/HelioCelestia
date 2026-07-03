#include "heliocelestia/io/render_style.hpp"

namespace heliocelestia::io {

RenderGlyphs RenderStyle::glyphs() const noexcept
{
    if (glyphMode == GlyphMode::Ascii) {
        return {
            .horizontal = "-",
            .topLeft = "+",
            .topRight = "+",
            .bottomLeft = "+",
            .bottomRight = "+",
            .vertical = "|"
        };
    }

    return {
        .horizontal = "─",
        .topLeft = "╭",
        .topRight = "╮",
        .bottomLeft = "╰",
        .bottomRight = "╯",
        .vertical = "│"
    };
}

} // namespace heliocelestia::io
