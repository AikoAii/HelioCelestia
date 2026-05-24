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
│   Observer, SolarPosition, SunriseResult            │
│   EquatorialCoordinate, HorizontalCoordinate        │
└─────────────────────────────────────────────────────┘
```

## Namespace Map

| Namespace | Purpose |
|---|---|
| `heliocelestia::core` | Primitive types, constants, DateTime |
| `heliocelestia::math` | Angle utilities, safe trig wrappers |
| `heliocelestia::models` | Plain data structs (no logic) |
| `heliocelestia::astronomy` | Pure calculation functions (no I/O, no state) |
| `heliocelestia::validation` | Input range checks |
| `heliocelestia::services` | Orchestration — wires astronomy modules together |
| `heliocelestia::io` | Output formatting (console and JSON) |
| `heliocelestia::cli` | Argument parsing and interactive prompt |

## Design Principles

- **Layered architecture** — each layer only depends on layers below it. `io` never includes `cli`, `astronomy` never includes `services`.
- **Pure functions** — all astronomy calculations are `noexcept` free functions with no global state.
- **SRP** — each file has one responsibility. `solar_model.cpp` computes equatorial coordinates. `refraction.cpp` applies the correction. `solar_service.cpp` orchestrates.
- **Zero external dependencies** — the engine is pure C++20 standard library. JSON is hand-rolled via `<sstream>`.
- **Extensibility** — adding a new celestial body (Moon, planets) means adding an `astronomy/lunar_model.cpp` and a new `services/lunar_service.cpp` without touching any existing file.
