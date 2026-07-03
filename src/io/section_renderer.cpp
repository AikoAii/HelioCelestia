#include "heliocelestia/io/section_renderer.hpp"

#include <algorithm>
#include <sstream>
#include <string>

namespace heliocelestia::io {

namespace {

std::string repeat(const std::string_view text, const int count)
{
    std::string out;
    for (int i = 0; i < count; ++i) {
        out += text;
    }
    return out;
}

std::string indent(const int count)
{
    return std::string(static_cast<std::size_t>(std::max(0, count)), ' ');
}

std::string centered(const std::string_view text, const int width)
{
    const int contentWidth = static_cast<int>(text.size());
    const int padding = std::max(0, width - contentWidth);
    const int left = padding / 2;
    const int right = padding - left;

    return std::string(static_cast<std::size_t>(left), ' ')
        + std::string(text)
        + std::string(static_cast<std::size_t>(right), ' ');
}

} // namespace

SectionRenderer::SectionRenderer(RenderStyle style)
    : style_ { style }
{
}

std::string SectionRenderer::banner(
    const std::string_view title,
    const std::string_view subtitle
) const
{
    const RenderGlyphs glyphs = style_.glyphs();
    const int innerWidth = style_.width - 2;

    std::ostringstream out;
    out << glyphs.topLeft << repeat(glyphs.horizontal, innerWidth) << glyphs.topRight << '\n';
    out << glyphs.vertical << centered(title, innerWidth) << glyphs.vertical << '\n';
    out << glyphs.vertical << centered(subtitle, innerWidth) << glyphs.vertical << '\n';
    out << glyphs.bottomLeft << repeat(glyphs.horizontal, innerWidth) << glyphs.bottomRight << "\n\n";
    return out.str();
}

std::string SectionRenderer::section(const std::string_view title) const
{
    std::ostringstream out;
    out << indent(style_.indent) << title << '\n';
    out << divider();
    return out.str();
}

std::string SectionRenderer::divider() const
{
    const RenderGlyphs glyphs = style_.glyphs();
    std::ostringstream out;
    out << indent(style_.indent)
        << repeat(glyphs.horizontal, style_.width - style_.indent)
        << '\n';
    return out.str();
}

} // namespace heliocelestia::io
