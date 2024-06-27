#ifndef P7_HPP_
#define P7_HPP_

#include <vector>
namespace ParkSpace {

int getInt(const char *str);
int getRandValue(int lower_bound, int upper_bound);
void printGameMap(const std::vector<char> &game_map, int size_of_env);
void initializePlayer(std::vector<char> &game_map, int size_of_env);
void initializeTrees(std::vector<char> &game_map, int size_of_env,
                     int num_of_trees);
void initializeBlueGrass(std::vector<char> &game_map, int size_of_env,
                         int num_of_bluegrass);

} // namespace ParkSpace

#endif
