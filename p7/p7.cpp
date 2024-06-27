#include "p7.hpp"
#include <cassert>
#include <cstddef>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <random>
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

int ParkSpace::getRandValue(int lower_bound, int upper_bound) {
  std::random_device r;
  std::default_random_engine engine(r());
  std::uniform_int_distribution<int> uniform_dist(lower_bound, upper_bound);
  int rand_value{uniform_dist(engine)};
  return rand_value;
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

void ParkSpace::initializePlayer(std::vector<char> &game_map, int size_of_env) {
  int cols{2};
  int num_of_spaces{cols * size_of_env};
  int rand_value{getRandValue(0, num_of_spaces - 1)};
  assert(rand_value < game_map.size());
  game_map.at(static_cast<std::size_t>(rand_value)) = 'P';
}

void ParkSpace::initializeTrees(std::vector<char> &game_map, int size_of_env,
                                int num_of_trees) {

  for (int i = 0; i < num_of_trees; ++i) {
    int rand_value{getRandValue(0, size_of_env * size_of_env)};
    assert(rand_value < game_map.size());

    // trees can spawn on top of other trees, fix...
    // Trees are initialized before bluegrass, so the
    // bluegrass initialization functio does all the
    // checks
    if (game_map.at(rand_value) == 'P') {
      game_map.at(rand_value + 1) = 'T';
    } else {
      game_map.at(rand_value) = 'T';
    }
  }
}

void ParkSpace::initializeBlueGrass(std::vector<char> &game_map,
                                    int size_of_env, int num_of_bluegrass) {

  for (int i = 0; i < num_of_bluegrass; ++i) {
    int rand_value{getRandValue(0, size_of_env * size_of_env)};
    assert(rand_value < game_map.size());

    if (game_map.at(rand_value) == 'P') {
      if (game_map.at(rand_value + 1) == 'T') {
        // combination of bluegrass '#' and tree 'T'
        game_map.at(rand_value + 1) = '*';
      } else {
        game_map.at(rand_value + 1) = '#';
      }
    } else if (game_map.at(rand_value) == 'T') {
      game_map.at(rand_value) = '*';
    } else {
      game_map.at(rand_value) = '#';
    }
  }
}
