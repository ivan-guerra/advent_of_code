#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

int GetNextLeftPlayer(const std::vector<bool>& players, int player) {
  int i = (player + 1) % players.size();
  while (!players[i]) {
    i = (i + 1) % players.size();
  }
  return i;
}

int GetNumOfWinningElf(int num_elves) {
  std::vector<bool> players(num_elves, true);
  int num_active_players = num_elves;

  int curr_player = 0;
  while (true) {
    int player_to_left = GetNextLeftPlayer(players, curr_player);
    players[player_to_left] = false;

    num_active_players--;
    if (num_active_players == 1) {
      return (curr_player + 1);
    }
    curr_player = GetNextLeftPlayer(players, curr_player);
  }
  return -1; /* unreachable */
}

int main(int argc, char** argv) {
  if (argc != 2) {
    std::cerr << "error: missing puzzle input file" << std::endl;
    exit(EXIT_FAILURE);
  }

  std::ifstream input(argv[1]);
  std::string num_elves;
  std::getline(input, num_elves);

  std::cout << "Answer: " << GetNumOfWinningElf(std::stoi(num_elves))
            << std::endl;

  exit(EXIT_SUCCESS);
}
