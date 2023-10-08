#include <cmath>
#include <cstdlib>
#include <fstream>
#include <functional>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>
#include <unordered_set>
#include <utility>
#include <vector>

using Position2D = std::pair<int, int>;

struct Position2DHash {
  std::size_t operator()(const Position2D& pos) const {
    std::string pos_str =
        std::to_string(pos.first) + "," + std::to_string(pos.second);
    return std::hash<std::string>{}(pos_str);
  }
};

struct Direction {
  char orientation = '\0';
  int num_steps = 0;
};

std::vector<Direction> ParseDirections(const std::string& directions) {
  std::vector<Direction> sequence;
  std::istringstream iss(directions);
  std::vector<std::string> tokens;
  std::copy(std::istream_iterator<std::string>(iss),
            std::istream_iterator<std::string>(), std::back_inserter(tokens));

  for (const std::string& token : tokens) {
    Direction dir;
    dir.orientation = token.front();
    if (token.back() == ',') {
      std::string trimmed = token.substr(0, token.size() - 1);
      dir.num_steps = std::stoi(trimmed.substr(1));
    } else {
      dir.num_steps = std::stoi(token.substr(1));
    }
    sequence.push_back(dir);
  }

  return sequence;
}

Position2D GetFinalLocation(const std::vector<Direction>& sequence) {
  enum class CardinalDir {
    kNorth,
    kSouth,
    kEast,
    kWest,
  };

  Position2D curr_pos(0, 0);
  CardinalDir curr_dir = CardinalDir::kNorth;
  std::unordered_set<Position2D, Position2DHash> visited;
  for (const Direction& d : sequence) {
    for (int i = 0; i < d.num_steps; ++i) {
      switch (curr_dir) {
        case CardinalDir::kNorth:
          curr_pos.first += (d.orientation == 'R') ? 1 : -1;
          break;
        case CardinalDir::kSouth:
          curr_pos.first += (d.orientation == 'R') ? -1 : 1;
          break;
        case CardinalDir::kEast:
          curr_pos.second += (d.orientation == 'R') ? -1 : 1;
          break;
        case CardinalDir::kWest:
          curr_pos.second += (d.orientation == 'R') ? 1 : -1;
          break;
      }
      if (visited.count(curr_pos)) {
        return curr_pos;
      }
      visited.insert(curr_pos);
    }
    switch (curr_dir) {
      case CardinalDir::kNorth:
        curr_dir =
            (d.orientation == 'R') ? CardinalDir::kEast : CardinalDir::kWest;
        break;
      case CardinalDir::kSouth:
        curr_dir =
            (d.orientation == 'R') ? CardinalDir::kWest : CardinalDir::kEast;
        break;
      case CardinalDir::kEast:
        curr_dir =
            (d.orientation == 'R') ? CardinalDir::kSouth : CardinalDir::kNorth;
        break;
      case CardinalDir::kWest:
        curr_dir =
            (d.orientation == 'R') ? CardinalDir::kNorth : CardinalDir::kSouth;
        break;
    }
  }
  return curr_pos;
}

int main(int argc, char** argv) {
  if (argc != 2) {
    std::cerr << "error: missing puzzle input file" << std::endl;
    exit(EXIT_FAILURE);
  }

  std::ifstream input(argv[1]);
  std::string directions;
  std::getline(input, directions);
  std::vector<Direction> sequence = ParseDirections(directions);

  Position2D final_pos = GetFinalLocation(sequence);
  int distance_to_hq = std::abs(final_pos.first) + std::abs(final_pos.second);
  std::cout << "Answer: " << distance_to_hq << std::endl;

  exit(EXIT_SUCCESS);
}
