# Scientific Foundations

All algorithms in HelioCelestia trace back to established published sources. This document explains the mathematics at an engineering level.

---

## 1. Julian Date (Meeus Ch.7)

Julian Date (JD) is a continuous count of days since noon on 1 January 4713 BC (Julian calendar). It provides a single monotonic number for any instant in history, avoiding calendar discontinuities.

**Formula:**

```
A = INT(Y / 100)
B = 2 − A + INT(A / 4)

JD = INT(365.25 × (Y + 4716))
   + INT(30.6001 × (M + 1))
   + D + B − 1524.5
```

Where `D` includes the fractional day from UTC hours. January and February are treated as months 13 and 14 of the previous year (Meeus convention).

**J2000 epoch:** JD 2451545.0 = 2000-01-01 12:00:00 UTC

---

## 2. Sidereal Time (Meeus Ch.12)

Greenwich Sidereal Time (GST) describes Earth's rotational orientation relative to distant stars (not the Sun). It advances ~3 min 56 sec faster per day than solar time.

**Formula (Meeus IAU):**

```
T = (JD − 2451545.0) / 36525          (Julian centuries since J2000)

GST = 280.46061837
    + 360.98564736629 × (JD − 2451545.0)
    + 0.000387933 × T²
    − T³ / 38710000                    (degrees, normalize to [0°, 360°))
```

**Local Sidereal Time:**
```
LST = GST + longitude_east             (normalize to [0°, 360°))
```

---

## 3. Solar Equatorial Coordinates (Meeus Ch.25)

A simplified but accurate model for the Sun's position on the celestial sphere.

```
n  = JD − 2451545.0                    (days since J2000)
L  = 280.460 + 0.9856474 × n          (mean solar longitude, degrees)
g  = 357.528 + 0.9856003 × n          (mean anomaly, degrees)
λ  = L + 1.915×sin(g) + 0.020×sin(2g) (ecliptic longitude)
ε  = 23.439281°                        (Earth's obliquity, J2000)

RA  = atan2(cos(ε)×sin(λ), cos(λ))    (right ascension)
Dec = asin(sin(ε)×sin(λ))             (declination)
```

**Accuracy:** ~0.01° for dates within ±200 years of J2000.

---

## 4. Equatorial → Horizontal Transform

Converts celestial (RA, Dec) into observer-local sky (Altitude, Azimuth).

```
HA = LST − RA                          (hour angle)

sin(Alt) = sin(Dec)×sin(φ) + cos(Dec)×cos(φ)×cos(HA)

Az = atan2(−sin(HA),
           tan(Dec)×cos(φ) − sin(φ)×cos(HA))
```

Where `φ` is the observer's geographic latitude. Azimuth is measured clockwise from North.

---

## 5. Atmospheric Refraction (Bennett 1982)

The atmosphere bends light downward, making the Sun appear higher than its geometric position. The correction `R` (degrees) is added to geometric altitude `h`:

```
R = (1.02 / tan(h + 10.3/(h + 5.11))) / 60
```

Where `h` is in degrees. Result is in degrees.

**Values at key altitudes:**

| h (geometric) | R (correction) |
|---|---|
| 0° (horizon) | ~0.48° |
| 10° | ~0.090° |
| 45° | ~0.017° |
| 90° (zenith) | ~0.000° |

Source: G.G. Bennett (1982), *Journal of Navigation* 35(2).

---

## 6. Sunrise / Sunset (NOAA Algorithm)

Based on the Wikipedia sunrise equation, which is a simplified form of Meeus Ch.15.

```
n*  = JD − 2451545.0 + 0.0008           (Julian cycle)
J*  = n* − (longitude / 360)            (approx. solar noon cycle)

M   = 357.5291 + 0.98560028 × n*        (mean anomaly)
C   = 1.9148×sin(M) + 0.0200×sin(2M) + 0.0003×sin(3M)
λ   = M + C + 180 + 102.9372            (ecliptic longitude)

J_transit = 2451545.0 + J* + 0.0053×sin(M) − 0.0069×sin(2λ)

δ   = arcsin(sin(λ) × sin(23.4397°))   (declination)

cos(ω₀) = (sin(−0.8333°) − sin(φ)×sin(δ)) / (cos(φ)×cos(δ))
```

The `−0.8333°` depression accounts for standard refraction (0.5667°) plus the solar disk semi-diameter (0.2667°).

- If `cos(ω₀) > 1`: **Polar night** — Sun never rises.
- If `cos(ω₀) < −1`: **Polar day** — Sun never sets.
- Otherwise: `J_rise = J_transit − ω₀/360`, `J_set = J_transit + ω₀/360`.

UTC hours from Julian date: `UTC = fmod(JD + 0.5, 1.0) × 24`.
