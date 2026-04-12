#include "SunPosition.h"
#include <cmath>
#include <sys/types.h>

const double PI = std::acos(-1.0);

double SunPosition::degToRad(double degrees) { return degrees * PI / 180.0; }

double SunPosition::radToDeg(double radians) { return radians * 180.0 / PI; }

double SunPosition::calculateAltitude(double DES, double HAS, double PHI) {
  double SD = std::sin(degToRad(DES));
  double CD = std::cos(degToRad(DES));
  double CH = std::cos(degToRad(HAS));
  double SP = std::sin(degToRad(PHI));
  double CP = std::cos(degToRad(PHI));

  double value = CD * CH * CP + SD * SP;

  // clamp
  if (value > 1.0)
    value = 1.0;
  if (value < -1.0)
    value = -1.0;

  return radToDeg(std::asin(value));
}

double SunPosition::calculateAzimuth(double DES, double HAS, double PHI) {
  double TD = std::tan(degToRad(DES));
  double SH = std::sin(degToRad(HAS));
  double CH = std::cos(degToRad(HAS));
  double SP = std::sin(degToRad(PHI));
  double CP = std::cos(degToRad(PHI));
  double A = radToDeg(std::atan2(SH, (CH * SP - TD * CP)));
  // return radToDeg(std::atan2(SH, (CH * SP - TD * CP)));

  A = A + 180.0; // konversi ke sistem kompas

  if (A < 0)
    A += 360.0;

  if (A >= 360)
    A -= 360.0;

  return A;
}

double SunPosition::calculateJulianDate(int year, int month, int day, int hour,
                                        int minute, int second) {
  if (month <= 2) {
    year -= 1;
    month += 12;
  }

  double A = std::floor(year / 100.0);
  double B = 2 - A + std::floor(A / 4.0);

  double fractionalDay =
      day + (hour + (minute / 60.0) + (second / 3600.0)) / 24.0;

  return std::floor(365.25 * (year + 4716)) +
         std::floor(30.6001 * (month + 1)) + fractionalDay + B - 1524.5;
}

double SunPosition::calculateDeclination(double JD) {
  double n = JD - 2451545.0;

  double g = 357.529 + 0.98560028 * n;
  double q = 280.459 + 0.98564736 * n;

  double L =
      q + 1.915 * std::sin(degToRad(g)) + 0.020 * std::sin(2 * degToRad(g));

  double e = 23.439 - 0.00000036 * n;

  return radToDeg(std::asin(std::sin(degToRad(e)) * std::sin(degToRad(L))));
}

double SunPosition::calculateHourAngle(int hour, int minute, int second,
                                       double longitude, double JD,
                                       double timezone) {

  double time = hour + (minute / 60.0) + (second / 3600.0);

  // Meridian dari timezone
  double standard_meridian = timezone * 15.0;
  double time_correction = (longitude - standard_meridian) / 15.0;

  // Equation of Time (EoT)
  double n = JD - 2451545.0;

  double B = 360.0 / 365.0 * (n - 81);

  double EoT = 9.87 * sin(degToRad(2 * B)) - 7.53 * cos(degToRad(B)) -
               1.5 * sin(degToRad(B));

  // convert menit ke jam
  double EoT_hours = EoT / 60.0;

  // true solar time
  double solar_time = time + time_correction + EoT_hours;

  // Hour Angle
  return 15.0 * (solar_time - 12.0);
}
