cmake_minimum_required(VERSION 3.20)

project(
    HelioCelestia

    VERSION 0.2.0

    DESCRIPTION
    "Scientific solar position calculation engine"

    LANGUAGES CXX
)

#
# C++ Standard
#

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

#
# Export compile commands for clangd / LSP
#

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# ──────────────────────────────────────────────────────────────
# Main Library
# ──────────────────────────────────────────────────────────────

add_library(heliocelestia)

target_sources(
    heliocelestia

    PRIVATE

    # Math
    src/math/angle.cpp

    # Astronomy
    src/astronomy/julian.cpp
    src/astronomy/sidereal.cpp
    src/astronomy/solar_model.cpp
    src/astronomy/coordinate_transform.cpp
    src/astronomy/refraction.cpp
    src/astronomy/sunrise_sunset.cpp

    # Services
    src/services/solar_service.cpp

    # Validation
    src/validation/coordinate_validator.cpp
    src/validation/datetime_validator.cpp

    # IO
    src/io/render_style.cpp
    src/io/section_renderer.cpp
    src/io/key_value_renderer.cpp
    src/io/formatter.cpp
    src/io/console_renderer.cpp
    src/io/json_formatter.cpp

    # CLI
    src/cli/argument_parser.cpp
)

target_include_directories(
    heliocelestia

    PUBLIC

    include
)

target_compile_options(
    heliocelestia

    PRIVATE

    -Wall
    -Wextra
    -Wpedantic
)

# ──────────────────────────────────────────────────────────────
# CLI Application
# ──────────────────────────────────────────────────────────────

add_executable(
    heliocelestia_cli

    apps/cli/main.cpp
)

target_link_libraries(
    heliocelestia_cli

    PRIVATE

    heliocelestia
)

# ──────────────────────────────────────────────────────────────
# Tests
# ──────────────────────────────────────────────────────────────

enable_testing()

# Helper macro to reduce boilerplate for each test binary.
macro(heliocelestia_test TARGET SOURCE)
    add_executable(${TARGET} ${SOURCE})
    target_link_libraries(${TARGET} PRIVATE heliocelestia)
    add_test(NAME ${TARGET} COMMAND ${TARGET})
endmacro()

# Unit tests
heliocelestia_test(angle_test                  tests/unit/angle_test.cpp)
heliocelestia_test(julian_test                 tests/unit/julian_test.cpp)
heliocelestia_test(sidereal_test               tests/unit/sidereal_test.cpp)
heliocelestia_test(solar_model_test            tests/unit/solar_model_test.cpp)
heliocelestia_test(coordinate_transform_test   tests/unit/coordinate_transform_test.cpp)
heliocelestia_test(refraction_test             tests/unit/refraction_test.cpp)
heliocelestia_test(sunrise_sunset_test         tests/unit/sunrise_sunset_test.cpp)
heliocelestia_test(datetime_validator_test     tests/unit/datetime_validator_test.cpp)
heliocelestia_test(console_renderer_test       tests/unit/console_renderer_test.cpp)

# Integration tests
heliocelestia_test(solar_position_test         tests/integration/solar_position_test.cpp)
heliocelestia_test(solar_position_full_test    tests/integration/solar_position_full_test.cpp)
