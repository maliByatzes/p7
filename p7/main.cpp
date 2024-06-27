#include "p7.hpp"
#include <cstdlib>
#include <iostream>
#include <vector>

int main(int argc, char *argv[]) {
  if (argc != 4) {
    std::cerr << "Invalid program invocation.\n";
    std::cout << "Run: " << argv[0]
              << " <SIZE_OF_ENV> <NUM_OF_TREES> <NUM_OF_BLUEGRASS>\n";
    return EXIT_FAILURE;
  }

  int size_of_env{};
  int num_of_trees{};
  int num_of_bluegrass{};

  try {
    size_of_env = ParkSpace::getInt(argv[1]);
    num_of_trees = ParkSpace::getInt(argv[2]);
    num_of_bluegrass = ParkSpace::getInt(argv[3]);
  } catch (const std::runtime_error &e) {
    std::cout << "Exiting: " << e.what() << '\n';
  }

  std::vector<char> game_map(size_of_env * size_of_env, '+');

  // game loop
  while (true) {
    // display game map
    ParkSpace::printGameMap(game_map, size_of_env);
    // handle player input
    char input{};
    std::cin >> input;
    // check for endgame
  }
}
