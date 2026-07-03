#include "heliocelestia/io/key_value_renderer.hpp"

#include <algorithm>
#include <iomanip>
#include <sstream>
#include <string>

namespace heliocelestia::io {

namespace {

std::string indent(const int count)
{
    return std::string(static_cast<std::size_t>(std::max(0, count)), ' ');
}

} // namespace

KeyValueRenderer::KeyValueRenderer(RenderStyle style)
    : style_ { style }
{
}

std::string KeyValueRenderer::render(
    const std::string_view key,
    const std::string_view value,
    const std::string_view unit,
    const std::string_view note
) const
{
    std::ostringstream out;

    out << indent(style_.indent)
        << std::left << std::setw(style_.labelWidth) << key
        << std::right << std::setw(style_.valueWidth) << value;

    if (!unit.empty()) {
        out << ' ' << unit;
    }

    if (!note.empty()) {
        out << "  " << note;
    }

    out << '\n';
    return out.str();
}

} // namespace heliocelestia::io
