#ifndef UTILS_H
#define UTILS_H

#include <string>

double getValidatedRangeInput(const std::string &prompt, double min,
                              double max); // Validasi Input
void clearScreen();                        // Kontrol layar
void clearInputBuffer();                   // Input buffer management
void printHeader();                        // UI header

#endif
