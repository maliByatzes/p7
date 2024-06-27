#include "p7.hpp"
#include <cstddef>
#include <cstdlib>
#include <iomanip>
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

void ParkSpace::printGameMap(const std::vector<char> &game_map,
                             int size_of_env) {

  for (std::size_t i = 0; i < size_of_env; ++i) {
    std::cout << std::setw(4);
    for (std::size_t j = 0; j < size_of_env; ++j) {
      std::cout << game_map.at(i + j * size_of_env) << ' ';
    }
    std::cout << '\n';
  }
  std::cout << '\n';
}
