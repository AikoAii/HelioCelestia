# Architecture

## Layer Diagram

```
┌─────────────────────────────────────────────────────┐
│                    apps/cli/                        │
│               main.cpp  (entry point)               │
└───────────────────────┬─────────────────────────────┘
                        │ uses
┌───────────────────────▼─────────────────────────────┐
│                  cli/  layer                        │
│           ArgumentParser, ParsedArguments           │
└───────────────────────┬─────────────────────────────┘
                        │ uses
┌───────────────────────▼─────────────────────────────┐
│               services/  layer                      │
│   SolarService — orchestrates all astronomy modules │
└──────┬──────────────┬──────────────┬────────────────┘
       │              │              │
┌──────▼──────┐ ┌─────▼─────┐ ┌────▼───────────────┐
│ astronomy/  │ │validation/│ │     io/  layer      │
│ julian      │ │ coord     │ │ Formatter           │
│ sidereal    │ │ datetime  │ │ ConsoleRenderer     │
│ solar_model │ └───────────┘ │ JsonFormatter       │
│ lunar_model │               │ SectionRenderer     │
│ coord_trans │               └────────────────────┘
│ refraction  │
│ sunrise_set │
└──────┬──────┘
       │ uses
┌──────▼──────────────────────────────────────────────┐
│              math/  layer                           │
│   angle (normalizeDegrees, degToRad, radToDeg)      │
│   trigonometry (safeAsin, safeAcos)                 │
└──────┬──────────────────────────────────────────────┘
       │ uses
┌──────▼──────────────────────────────────────────────┐
│              core/  layer                           │
│   DateTime, precision types (Real, Float64…)        │
│   constants (kPi, kEarthObliquity…)                 │
└─────────────────────────────────────────────────────┘
       │ (data types only — no logic)
┌──────▼──────────────────────────────────────────────┐
│              models/  layer                         │
│   Observer, SolarPosition, LunarPosition            │
│   SunriseResult                                     │
│   EquatorialCoordinate, HorizontalCoordinate        │
└─────────────────────────────────────────────────────┘
```

## Namespace Map

| Namespace | Purpose |
|---|---|
| `heliocelestia::core` | Primitive types, constants, DateTime |
| `heliocelestia::math` | Angle utilities, safe trig wrappers |
| `heliocelestia::models` | Plain data structs (no logic) |
| `heliocelestia::astronomy` | Pure calculation functions (no I/O, no state), including solar and lunar models |
| `heliocelestia::validation` | Input range checks |
| `heliocelestia::services` | Orchestration — wires astronomy modules together |
| `heliocelestia::io` | Output formatting (console and JSON) |
| `heliocelestia::cli` | Argument parsing and interactive prompt |

## Design Principles

- **Layered architecture** — each layer only depends on layers below it. `io` never includes `cli`, `astronomy` never includes `services`.
- **Pure functions** — all astronomy calculations are `noexcept` free functions with no global state.
- **SRP** — each file has one responsibility. `solar_model.cpp` computes equatorial coordinates. `refraction.cpp` applies the correction. `solar_service.cpp` orchestrates.
- **Zero external dependencies** — the engine is pure C++20 standard library. JSON is hand-rolled via `<sstream>`.
- **Extensibility** — adding a new celestial body starts with a focused `astronomy/*_model.cpp` module and a service entry point. As feature depth grows, body-specific services such as `LunarService` or `PlanetaryService` can be introduced without changing lower-level astronomy modules.

## v2.0 Moon Position

Moon position support is implemented as a small astronomy module that reuses
the existing time and coordinate transformation pipeline:

```
apps/cli/main.cpp
        │
        ▼
services/SolarService::calculateLunarPosition
        │
        ├── astronomy/calculateJulianDate
        ├── astronomy/calculateGreenwichSiderealTime
        ├── astronomy/calculateLunarEquatorialCoordinate
        └── astronomy/transformEquatorialToHorizontal
```

The lunar model is display-oriented and dependency-free. It computes an
approximate geocentric lunar ecliptic position, converts it to equatorial
coordinates, and then projects it into observer-local altitude/azimuth.
