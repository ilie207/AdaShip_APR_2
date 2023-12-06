#include "Utils.h"
#include <sstream>
#include <string>
#include <iostream>

std::string convertToCoordinate(int row, int col) {
  char letter = static_cast<char>('A' + col);
  std::ostringstream oss;
  oss << letter << row;
  return oss.str();
}
