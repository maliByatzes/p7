#ifndef P7_HPP_
#define P7_HPP_

#include <termios.h>
#include <vector>

namespace ParkSpace {

int getInt(const char *str);
int getRandValue(int lower_bound, int upper_bound);
void enableRawMode();
void disableRawMode();
void die(const char* s);
void printGameMap(const std::vector<char> &game_map, int size_of_env);
void printKey();
void initializePlayer(std::vector<char> &game_map, int size_of_env);
void initializeTrees(std::vector<char> &game_map, int size_of_env,
                     int num_of_trees);
void initializeBlueGrass(std::vector<char> &game_map, int size_of_env,
                         int num_of_bluegrass);

} // namespace ParkSpace

#endif
