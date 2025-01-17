#include "p7.hpp"
#include <algorithm>
#include <cassert>
#include <cctype>
#include <cerrno>
#include <cmath>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <sstream>
#include <stdexcept>
#include <termios.h>
#include <unistd.h>
#include <vector>

#define CRTL_KEY(k) ((k) & 0x1f)

struct termios original_termios;

namespace ParkSpace {

int getInt(const char *str) {
  int num{};
  std::stringstream ss{str};
  ss >> num;
  if (ss.fail()) {
    throw std::runtime_error("Could not convert from string to integer.");
  }
  return num;
}

int getRandValue(int lower_bound, int upper_bound) {
  assert(upper_bound > lower_bound);
  int range{upper_bound - lower_bound + 1};
  return std::rand() % range + lower_bound;
}

void enableRawMode() {
  if (tcgetattr(STDIN_FILENO, &original_termios) == -1) {
    die("tcgetattr");
  }
  atexit(disableRawMode);

  struct termios raw = original_termios;
  raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
  // raw.c_oflag &= ~(OPOST);
  raw.c_cflag |= (CS8);
  raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);

  if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1) {
    die("tcsetattr");
  }
}

void disableRawMode() {
  if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &original_termios) == -1) {
    die("tcsetattr");
  }
}

void die(const char *s) {
  std::cout << "\x1b[2J";
  std::cout << "\x1b[H";

  perror(s);
  std::exit(1);
}

void gameProcessKeypress(std::vector<char> &game_map, int size_of_env) {
  char c{readSingleKey()};

  switch (std::tolower(c)) {
  case 'w':
    movePlayerUp(game_map, size_of_env);
    break;
  case 'a':
    movePlayerLeft(game_map, size_of_env);
    break;
  case 'd':
    movePlayerRight(game_map, size_of_env);
    break;
  case 's':
    movePlayerDown(game_map, size_of_env);
    break;
  case CRTL_KEY('q'):
    std::cout << "\x1b[2J";
    std::cout << "\x1b[H";
    std::exit(0);
    break;
  }
}

char readSingleKey() {
  int nread{};
  char c{};
  while ((nread = read(STDIN_FILENO, &c, 1)) != 1) {
    if (nread == -1 && errno != EAGAIN) {
      die("read");
    }
  }
  return c;
}

void clearScreen() {
  std::cout << "\x1b[2J";
  std::cout << "\x1b[H";
}

void printGameMap(const std::vector<char> &game_map, int size_of_env) {

  for (std::size_t i = 0; i < size_of_env; ++i) {
    std::cout << std::setw(4);
    for (std::size_t j = 0; j < size_of_env; ++j) {
      std::cout << game_map.at(i + j * size_of_env) << ' ';
    }
    std::cout << '\n';
  }
  std::cout << '\n';
}

void printKey() {
  std::cout << "+++++++++++++KEY+++++++++++++\n";
  std::cout << "P => Player, T => Tree, I = Infested Tree\n";
  std::cout << "# => Bluegrass, * => Tree on Bluegrass\n";
  std::cout << "_ => Lawn\n";
}

void initializePlayer(std::vector<char> &game_map, int size_of_env) {
  int cols{2};
  int num_of_spaces{cols * size_of_env};
  int rand_value{getRandValue(0, num_of_spaces - 1)};
  assert(rand_value < game_map.size());
  game_map.at(static_cast<std::size_t>(rand_value)) = 'P';
}

void initializeTrees(std::vector<char> &game_map, int size_of_env,
                     int num_of_trees) {

  // TODO: some bug is here about random values

  double num_of_infested{std::ceil(num_of_trees * 0.25)};
  int infested_trees{static_cast<int>(num_of_infested)};

  for (int i = 0; i < num_of_trees - infested_trees; ++i) {
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

  for (int i = 0; i < infested_trees; ++i) {
    int rand_value{getRandValue(0, size_of_env * size_of_env)};
    assert(rand_value < game_map.size());

    if (game_map.at(rand_value) == 'P') {
      game_map.at(rand_value + 1) = 'I';
    } else {
      game_map.at(rand_value) = 'I';
    }
  }
}

void initializeBlueGrass(std::vector<char> &game_map, int size_of_env,
                         int num_of_bluegrass) {

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

    } else if (game_map.at(rand_value) == 'I') {
      continue;
    } else {
      game_map.at(rand_value) = '#';
    }
  }
}

// absolutely epitome of good code this 󰩐
void updateGameMap(std::vector<char> &game_map, int size_of_env) {
  // update lawn with bluegrass
  for (size_t i = 0; i < size_of_env * size_of_env; ++i) {
    if (game_map.at(i) == '_') {

      // first column
      if (i < size_of_env) {
        // top-left corner
        if (i == 0) {
          int cumulative{};
          if (game_map.at(i + 3) == '#') {
            cumulative += 5;
          }

          if (game_map.at(i + 1) == '#') {
            cumulative += 5;
          }
          // chance spawn bluegrass at i
          continue;
        }
        // bottom-left corner
        else if (i == (size_of_env - 1)) {
          int cumulative{};
          if (game_map.at(i - 1) == '#') {
            cumulative += 5;
          }

          if (game_map.at(i + 3) == '#') {
            cumulative += 5;
          }
          // chance spawn bluegrass at i
          continue;
        } else {
          int cumulative{};
          if (game_map.at(i - 1) == '#') {
            cumulative += 5;
          }

          if (game_map.at(i + 3) == '#') {
            cumulative += 5;
          }

          if (game_map.at(i + 1) == '#') {
            cumulative += 5;
          }
          // chance spawn bluegrass at i
          continue;
        }
      }

      // first row
      if (isMultipleOf(i, size_of_env) && i != 0) {
        // top-right corner
        if (i == ((size_of_env * size_of_env) - size_of_env)) {
          int cumulative{};
          if (game_map.at(i - 3) == '#') {
            cumulative += 5;
          }

          if (game_map.at(i + 1) == '#') {
            cumulative += 5;
          }
          // chance spawn bluegrass at i
          continue;
        } else {
          int cumulative{};
          if (game_map.at(i - 3) == '#') {
            cumulative += 5;
          }
          if (game_map.at(i + 1) == '#') {
            cumulative += 5;
          }
          if (game_map.at(i + 3) == '#') {
            cumulative += 5;
          }
          // chance spawn bluegrass at i
          continue;
        }
      }

      // last column
      if (i > ((size_of_env * size_of_env) - size_of_env)) {
        // bottom-left corner
        if (i == ((size_of_env * size_of_env) - 1)) {
          int cumulative{};
          if (game_map.at(i - 1) == '#') {
            cumulative += 5;
          }

          if (game_map.at(i - 3) == '#') {
            cumulative += 5;
          }
          // chance spawn bluegrass at i
          continue;
        } else {
          int cumulative{};
          if (game_map.at(i - 1) == '#') {
            cumulative += 5;
          }
          if (game_map.at(i - 3) == '#') {
            cumulative += 5;
          }
          if (game_map.at(i + 1) == '#') {
            cumulative += 5;
          }
          // chance spawn bluegrass at i
          continue;
        }
      }

      // last row
      if (isMultipleOf(i + 1, size_of_env) && i != (size_of_env - 1) &&
          i != ((size_of_env * size_of_env) - 1)) {
        int cumulative{};
        if (game_map.at(i - 1) == '#') {
          cumulative += 5;
        }
        if (game_map.at(i - 3) == '#') {
          cumulative += 5;
        }
        if (game_map.at(i + 3) == '#') {
          cumulative += 5;
        }
        // chance spawn bluegrass at i
        continue;
      }

      // middle
      int cumulative{};
      if (game_map.at(i - 1) == '#') {
        cumulative += 5;
      }
      if (game_map.at(i - 3) == '#') {
        cumulative += 5;
      }
      if (game_map.at(i + 1) == '#') {
        cumulative += 5;
      }
      if (game_map.at(i + 3) == '#') {
        cumulative += 5;
      }
      // chance spawn bluegrass at i
    }
  }
}

void movePlayerUp(std::vector<char> &game_map, int size_of_env) {
  int player_idx{};
  try {
    player_idx = findPlayerPos(game_map);
  } catch (std::runtime_error &e) {
    std::cout << e.what() << '\n';
    std::exit(1);
  }

  if (player_idx != 0 && !isMultipleOf(player_idx, size_of_env)) {
    int player_dest{player_idx - 1};

    if (game_map.at(player_dest) != 'T') {
      if (game_map.at(player_dest) == 'I' || game_map.at(player_dest) == '*') {
        game_map.at(player_dest) = 'T';
      } else {
        game_map.at(player_idx) = '_';
        game_map.at(player_dest) = 'P';
      }
    }
  }
}

void movePlayerDown(std::vector<char> &game_map, int size_of_env) {
  int player_idx{};
  try {
    player_idx = findPlayerPos(game_map);
  } catch (std::runtime_error &e) {
    std::cout << e.what() << '\n';
  }

  if (!isMultipleOf(player_idx + 1, size_of_env)) {
    int player_dest{player_idx + 1};

    if (game_map.at(player_dest) != 'T') {
      if (game_map.at(player_dest) == 'I' || game_map.at(player_dest) == '*') {
        game_map.at(player_dest) = 'T';
      } else {
        game_map.at(player_idx) = '_';
        game_map.at(player_dest) = 'P';
      }
    }
  }
}

void movePlayerLeft(std::vector<char> &game_map, int size_of_env) {
  int player_idx{};
  try {
    player_idx = findPlayerPos(game_map);
  } catch (std::runtime_error &e) {
    std::cout << e.what() << '\n';
  }

  if (player_idx >= size_of_env) {
    int player_dest{player_idx - size_of_env};

    if (game_map.at(player_dest) != 'T') {
      if (game_map.at(player_dest) == 'I' || game_map.at(player_dest) == '*') {
        game_map.at(player_dest) = 'T';
      } else {
        game_map.at(player_idx) = '_';
        game_map.at(player_dest) = 'P';
      }
    }
  }
}

void movePlayerRight(std::vector<char> &game_map, int size_of_env) {
  int player_idx{};
  try {
    player_idx = findPlayerPos(game_map);
  } catch (std::runtime_error &e) {
    std::cout << e.what() << '\n';
  }

  if (player_idx < ((size_of_env * size_of_env) - size_of_env)) {
    int player_dest{player_idx + size_of_env};

    if (game_map.at(player_dest) != 'T') {
      if (game_map.at(player_dest) == 'I' || game_map.at(player_dest) == '*') {
        game_map.at(player_dest) = 'T';
      } else {
        game_map.at(player_idx) = '_';
        game_map.at(player_dest) = 'P';
      }
    }
  }
}

bool isMultipleOf(int a, int b) {
  if (b != 0) {
    if (a % b == 0) {
      return true;
    }
  }
  return false;
}

int findPlayerPos(const std::vector<char> &game_map) {
  auto player_it = std::find(game_map.begin(), game_map.end(), 'P');
  if (player_it == std::end(game_map)) {
    throw std::runtime_error("Could not find player in game map");
  }
  return static_cast<int>(player_it - game_map.begin());
}
} // namespace ParkSpace
