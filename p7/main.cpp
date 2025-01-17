#include "p7.hpp"
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <stdexcept>
#include <vector>

int main(int argc, char *argv[]) {
  std::srand(static_cast<unsigned int>(std::time(0)));

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

    if (size_of_env < 5) {
      throw std::runtime_error("Size of canvas must at least 5");
    }
  } catch (const std::runtime_error &e) {
    std::cout << "Exiting: " << e.what() << '\n';
    std::exit(1);
  }

  std::vector<char> game_map(size_of_env * size_of_env, '_');

  // Initialize game map with entities
  ParkSpace::initializePlayer(game_map, size_of_env);
  ParkSpace::initializeTrees(game_map, size_of_env, num_of_trees);
  ParkSpace::initializeBlueGrass(game_map, size_of_env, num_of_bluegrass);

  ParkSpace::enableRawMode();

  while (true) {
    ParkSpace::clearScreen();

    ParkSpace::printGameMap(game_map, size_of_env);
    ParkSpace::printKey();

    ParkSpace::gameProcessKeypress(game_map, size_of_env);

    ParkSpace::updateGameMap(game_map, size_of_env);
  }
}
