#include "p7.hpp"
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <stdexcept>

int ParkSpace::getInt(const char *str) {
  int num{};
  std::stringstream ss{str};
  ss >> num;
  if (ss.fail()) {
    throw std::runtime_error("Could not convert from string to integer.");
  }
  return num;
}
