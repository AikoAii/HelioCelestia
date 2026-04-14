#include "SunPosition.h" // Header posisi matahari
#include "utils.h"
#include <ctime>
#include <iostream>
#include <map>
#include <string>

std::map<std::string, double> parseArgs(int argc, char *argv[]) {
  std::map<std::string, double> args;

  for (int i = 1; i < argc; i += 2) {
    std::string key = argv[i];
    if (i + 1 < argc)
      args[key] = std::stod(argv[i + 1]);
  }

  return args;
}

int main(int argc, char *argv[]) {

  auto args = parseArgs(argc, argv);

  bool jsonMode = false;

  for (int i = 0; i < argc; ++i) {
    if (std::string(argv[i]) == "--json")
      jsonMode = true;
  }

  // Inisialisasi
  double PHI, DES, HAS, JD, LON, TZ;
  char yn;

  // =========================
  // MODE CLI
  // =========================
  if (argc > 1) {

    if (!args.count("--lat") || !args.count("--lon") || !args.count("--tz")) {
      std::cout << "Usage:\n";
      std::cout << "./heliocelestia --lat <value> --lon <value> --tz <value>\n";
      return 1;
    }

    PHI = args["--lat"];
    LON = args["--lon"];
    TZ = args["--tz"];

    if (PHI < -90 || PHI > 90) {
      std::cout << "Latitude harus antara -90 sampai 90\n";
      return 1;
    }

    if (LON < -180 || LON > 180) {
      std::cout << "Longitude harus antara -180 sampai 180\n";
      return 1;
    }

    if (TZ < -12 || TZ > 14) {
      std::cout << "Timezone harus antara -12 sampai 14\n";
      return 1;
    }

    // Waktu sistem
    std::time_t now = std::time(0);
    std::tm *ltm = std::localtime(&now);

    int year = 1900 + ltm->tm_year;
    int month = 1 + ltm->tm_mon;
    int day = ltm->tm_mday;
    int hour = ltm->tm_hour;
    int minute = ltm->tm_min;
    int second = ltm->tm_sec;

    // Hitung kalender julian
    JD = SunPosition::calculateJulianDate(year, month, day, hour, minute,
                                          second);

// Debug output
#ifdef DEBUG
    std::cout << "Julian Date: " << JD << "\n";
#endif // !DEBUG

    // Hitung deklinasi
    DES = SunPosition::calculateDeclination(JD);

    // Hitung sudut jam
    HAS = SunPosition::calculateHourAngle(hour, minute, second, LON, JD, TZ);

    // Hitung posisi matahari
    double ALT = SunPosition::calculateAltitude(DES, HAS, PHI);
    double AZI = SunPosition::calculateAzimuth(DES, HAS, PHI);

    // Hasil
    if (jsonMode) {
      std::cout << "{\n";
      std::cout << "  \"altitude\": " << ALT << ",\n";
      std::cout << "  \"azimuth\": " << AZI << "\n";
      std::cout << "}\n";
    } else {
      std::cout << "\n------------------ HASIL ------------------\n";
      std::cout << "Tinggi Sudut Matahari (ALT) : " << ALT << " derajat\n";
      std::cout << "Azimuth Matahari (AZI)      : " << AZI << " derajat\n";
      std::cout << "--------------------------------------------\n\n";
    }

    return 0;
  }

  // =========================
  // MODE INTERACTIVE
  // =========================

  // loop
  do {
    printHeader();

    // Waktu sistem
    std::time_t now = std::time(0);
    std::tm *ltm = std::localtime(&now);

    int year = 1900 + ltm->tm_year;
    int month = 1 + ltm->tm_mon;
    int day = ltm->tm_mday;
    int hour = ltm->tm_hour;
    int minute = ltm->tm_min;
    int second = ltm->tm_sec;

    // Hitung kalender julian
    JD = SunPosition::calculateJulianDate(year, month, day, hour, minute,
                                          second);

// Debug output
#ifdef DEBUG
    std::cout << "Julian Date: " << JD << "\n";
#endif

    // Hitung deklinasi
    DES = SunPosition::calculateDeclination(JD);
    std::cout << "Deklinasi Matahari: " << DES << "\n";

    // Input Latitude dan Longitude
    PHI = getValidatedRangeInput(
        "Masukkan Latitude Pengamat [PHI] (-90 s/d 90): ", -90.0, 90.0);
    LON = getValidatedRangeInput(
        "Masukkan Longitude Pengamat [LON] (-180 s/d 180): ", -180.0, 180.0);
    TZ =
        getValidatedRangeInput("Masukkan Timezone (-12 s/d 14): ", -12.0, 14.0);

    // Hitung sudut jam
    HAS = SunPosition::calculateHourAngle(hour, minute, second, LON, JD, TZ);
    std::cout << "Hour Angle: " << HAS << "\n";

    // Hitung posisi matahari
    double ALT = SunPosition::calculateAltitude(DES, HAS, PHI);
    double AZI = SunPosition::calculateAzimuth(DES, HAS, PHI);

    // Hasil
    std::cout << "\n------------------ HASIL ------------------\n";
    std::cout << "Tinggi Sudut Matahari (ALT) : " << ALT << " derajat\n";
    std::cout << "Azimuth Matahari (AZI)      : " << AZI << " derajat\n";
    std::cout << "--------------------------------------------\n\n";

    // Kontrol loop
    std::cout << "Mau hitung lagi? (y/n): ";
    std::cin >> yn;

    clearInputBuffer();

  } while (yn == 'Y' || yn == 'y');

  // END
  std::cout << "Program selesai. Sampai jumpa lagi :3!\n";
  return 0;
}
