#ifndef SUN_POSITION_H
#define SUN_POSITION_H

class SunPosition {
private:
  static double degToRad(double degrees); // derajat ke radian
  static double radToDeg(double radians); // radian ke derajat

public:
  static double calculateAltitude(double DES, double HAS,
                                  double PHI); // altitude
  static double calculateAzimuth(double DES, double HAS, double PHI); // Azimuth
  static double calculateJulianDate(int year, int month, int day, int hour,
                                    int minute, int second); // Julian Date
  static double calculateDeclination(double JD); // Deklinasi matahari
  static double calculateHourAngle(int hour, int minute, int second,
                                   double longitude, double JD,
                                   double timezone);             // Sudut Jam
  static double calculatePrediction(double JD, double timezone); // Prediksi
};

#endif
