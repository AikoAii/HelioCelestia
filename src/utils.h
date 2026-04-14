#ifndef UTILS_H
#define UTILS_H

#include <string>

double getValidatedRangeInput(const std::string &prompt, double min,
                              double max);
void clearScreen();
void clearInputBuffer();
void printHeader();

#endif
