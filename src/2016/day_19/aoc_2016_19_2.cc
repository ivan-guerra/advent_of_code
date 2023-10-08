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

int BinarySearch(const std::vector<int>& values, int key) {
  int l = 0;
  int r = values.size();
  while (l <= r) {
    int m = (l + r) / 2;
    if (values[m] == key) {
      return m;
    } else if (values[m] < key) {
      l = m + 1;
    } else {
      r = m - 1;
    }
  }
  return -1;
}

int GetPlayerAcrossCircle(std::vector<int>& players_circle, int player) {
  int dist = players_circle.size() / 2;
  int deletion_index =
      (BinarySearch(players_circle, player) + dist) % players_circle.size();
  int player_across = players_circle[deletion_index];

  players_circle.erase(players_circle.begin() + deletion_index);

  return player_across;
}

int GetNumOfWinningElf(int num_elves) {
  std::vector<bool> players(num_elves, true);
  std::vector<int> players_circle(num_elves);
  for (int i = 0; i < num_elves; ++i) {
    players_circle[i] = i;
  }
  int curr_player = 0;
  while (true) {
    int player_across = GetPlayerAcrossCircle(players_circle, curr_player);
    players[player_across] = false;

    if (players_circle.size() == 1) {
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
