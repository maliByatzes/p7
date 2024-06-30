#include "p7.hpp"
#include <cctype>
#include <cerrno>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <termios.h>
#include <unistd.h>
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

  std::vector<char> game_map(size_of_env * size_of_env, '_');

  // Initialize game map with entities
  ParkSpace::initializePlayer(game_map, size_of_env);
  ParkSpace::initializeTrees(game_map, size_of_env, num_of_trees);
  ParkSpace::initializeBlueGrass(game_map, size_of_env, num_of_bluegrass);

  ParkSpace::enableRawMode();

  char c{};
  while (true) {
    ParkSpace::printGameMap(game_map, size_of_env);
    ParkSpace::printKey();
    if (read(STDIN_FILENO, &c, 1) == -1 && errno != EAGAIN) {
      ParkSpace::die("read");
    }
    if (std::iscntrl(c)) {
      printf("%d\n", c);
    } else {
      printf("%d ('%c')\n", c, c);
    }
    if (c == 'q') {
      break;
    }
  }

  /*
  // game loop
  while (true) {
    // display game map
    ParkSpace::printGameMap(game_map, size_of_env);
    ParkSpace::printKey();
    // handle player input
    // check for endgame
  }*/
}
