#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_set>
#include <vector>

struct Tile {
  int value = 0;
  bool marked = false;
};

using BingoBoard = std::vector<std::vector<Tile>>;
using BingoBoardVec = std::vector<BingoBoard>;
using BingoCallouts = std::vector<int>;

[[nodiscard]] BingoCallouts ParseCallouts(std::ifstream& fhandle) {
  /* read in the single line containing the callouts */
  std::string callouts_str;
  std::getline(fhandle, callouts_str);

  /* parse the comma delimited callouts line */
  std::stringstream ss(callouts_str);
  std::string value;
  BingoCallouts callouts;
  while (std::getline(ss, value, ',')) {
    callouts.push_back(std::stoi(value));
  }
  return callouts;
}

[[nodiscard]] BingoBoardVec ParseBingoBoards(std::ifstream& fhandle) {
  /* consume blank line following callouts */
  std::string line;
  std::getline(fhandle, line);

  BingoBoardVec boards;
  BingoBoard board;
  while (std::getline(fhandle, line)) {
    if (line.empty()) {
      boards.push_back(board);
      board.clear();
    } else {
      std::vector<Tile> tiles(5);
      std::sscanf(line.c_str(), "%d %d %d %d %d", &tiles[0].value,
                  &tiles[1].value, &tiles[2].value, &tiles[3].value,
                  &tiles[4].value);
      board.push_back(tiles);
    }
  }
  /* save the final board (doesn't have a blank following it in the input)*/
  boards.push_back(board);

  return boards;
}

[[nodiscard]] int SumAllUnmarkedTiles(const BingoBoard& board) {
  int unmarked_sum = 0;
  for (const auto& row : board) {
    for (const Tile& tile : row) {
      if (!tile.marked) {
        unmarked_sum += tile.value;
      }
    }
  }
  return unmarked_sum;
}

[[nodiscard]] int GetWinnerScore(const BingoBoard& board) noexcept {
  /* scan row by row for a winner */
  bool found_winner = true;
  for (int i = 0; i < board.size(); ++i) {
    for (int j = 0; j < board[i].size() && found_winner; ++j) {
      if (!board[i][j].marked) {
        found_winner = false;
        break;
      }
    }

    if (found_winner) {
      return SumAllUnmarkedTiles(board);
    }
    found_winner = true;
  }

  /* scan col by col for a winner */
  for (int i = 0; i < board[i].size(); ++i) {
    for (int j = 0; j < board.size() && found_winner; ++j) {
      if (!board[j][i].marked) {
        found_winner = false;
        break;
      }
    }

    if (found_winner) {
      return SumAllUnmarkedTiles(board);
    }
    found_winner = true;
  }

  return 0;
}

[[nodiscard]] int FindLastWinningBoard(const BingoCallouts& callouts,
                                       BingoBoardVec& boards) noexcept {
  int last_winner_score = 0;
  std::unordered_set<int> winner_board_indices;
  for (const int& callout : callouts) {
    for (int i = 0; i < boards.size(); ++i) {
      /* only update a board if it hasn't won yet */
      if (!winner_board_indices.count(i)) {
        for (auto& row : boards[i]) {
          for (auto& tile : row) {
            if (tile.value == callout) {
              tile.marked = true;
            }

            int winner_score = GetWinnerScore(boards[i]);
            if (winner_score) {
              /* we have a new winner */
              winner_board_indices.insert(i);
              last_winner_score = (winner_score * callout);
            }
          }
        }
      }
    }
  }
  return last_winner_score;
}

int main(int argc, char** argv) {
  if (argc != 2) {
    std::cerr << "error: missing puzzle input file" << std::endl;
    exit(EXIT_FAILURE);
  }

  std::ifstream fhandle(argv[1]);
  BingoCallouts callouts = ParseCallouts(fhandle);
  BingoBoardVec boards = ParseBingoBoards(fhandle);
  std::cout << "Answer: " << FindLastWinningBoard(callouts, boards)
            << std::endl;

  exit(EXIT_SUCCESS);
}
