#include "utils.h"
#include <cstdlib>
#include <iostream>
#include <limits>

// Validasi input
double getValidatedRangeInput(const std::string &prompt, double min,
                              double max) {
  double value;
  while (true) {
    std::cout << prompt;
    if (std::cin >> value && value >= min && value <= max)
      return value;

    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cout << "Input harus dalam range [" << min << ", " << max << "]!\n\n";
  }
}

// Kontrol layar
void clearScreen() {
#if defined(_WIN32)
  system("CLS");
#else
  system("clear");
#endif
}

// Pembersih buffer input
void clearInputBuffer() {
  std::cin.clear();
  std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

// UI Header
void printHeader() {
  clearScreen();
  std::cout << "========================================================\n";
  std::cout << "  Program Perhitungan Tinggi Sudut Dan Azimuth Matahari \n";
  std::cout << "========================================================\n";
  std::cout << "  MODE : OTOMATIS (Waktu Sistem)\n";
  std::cout << "--------------------------------------------------------\n";
  std::cout << "  INPUT USER\n";
  std::cout << "  Latitude Pengamat (deg)                      PHI\n\n";
  std::cout << "  HITUNG OTOMATIS\n";
  std::cout << "  Deklinasi Matahari (deg)                    DES\n";
  std::cout << "  Sudut Jam Matahari (deg)                    HAS\n";
  std::cout << "========================================================\n\n";
}
