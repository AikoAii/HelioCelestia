#pragma once

#include <string_view>

namespace heliocelestia::io {

enum class GlyphMode {
    Unicode,
    Ascii
};

struct RenderGlyphs {
    std::string_view horizontal;
    std::string_view topLeft;
    std::string_view topRight;
    std::string_view bottomLeft;
    std::string_view bottomRight;
    std::string_view vertical;
};

struct RenderStyle {
    GlyphMode glyphMode { GlyphMode::Unicode };
    int width { 48 };
    int labelWidth { 26 };
    int valueWidth { 13 };
    int indent { 2 };

    [[nodiscard]] RenderGlyphs glyphs() const noexcept;
};

} // namespace heliocelestia::io
