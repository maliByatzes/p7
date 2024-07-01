#ifndef P7_HPP_
#define P7_HPP_

#include <vector>

namespace ParkSpace {

int getInt(const char *str);
int getRandValue(int lower_bound, int upper_bound);
void enableRawMode();
void disableRawMode();
void die(const char *s);
char readSingleKey();
void gameProcessKeypress(std::vector<char> &game_map, int size_of_env);
void clearScreen();
void printGameMap(const std::vector<char> &game_map, int size_of_env);
void printKey();
void initializePlayer(std::vector<char> &game_map, int size_of_env);
void initializeTrees(std::vector<char> &game_map, int size_of_env,
                     int num_of_trees);
void initializeBlueGrass(std::vector<char> &game_map, int size_of_env,
                         int num_of_bluegrass);

void movePlayerUp(std::vector<char> &game_map, int size_of_env);
void movePlayerDown(std::vector<char> &game_map, int size_of_env);

bool isMultipleOf(int a, int b);
int findPlayerPos(const std::vector<char> &game_map);

} // namespace ParkSpace

#endif
