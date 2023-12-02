#include <cstdlib>
#include <fstream>
#include <iostream>
#include <numeric>
#include <sstream>
#include <string>
#include <vector>

struct Record;
struct Draw;
struct Game;

using RecordVec = std::vector<Record>;
using DrawVec = std::vector<Draw>;
using GameVec = std::vector<Game>;

enum class Color { kRed, kGreen, kBlue };

struct Record {
  int count = 0;
  Color cube_color;
};

struct Draw {
  RecordVec records;
};

struct Game {
  int id = 0;
  DrawVec subsets;
};

struct RgbLimit {
  int red = 12;
  int green = 13;
  int blue = 14;
};

[[nodiscard]] Draw ParseDraw(const std::string& draw_str) {
  auto GetColorType = [](const std::string& color_str) {
    if (color_str == "red") {
      return Color::kRed;
    } else if (color_str == "blue") {
      return Color::kBlue;
    } else {
      return Color::kGreen;
    }
  };

  Draw draw;
  Record current_record;
  bool is_count = true;
  std::string token;
  std::istringstream iss(draw_str);
  while (std::getline(iss, token, ' ')) {
    if (is_count) {
      current_record.count = std::stoi(token);
      is_count = false;
    } else {
      if (token.back() == ',') {
        token = token.substr(0, token.size() - 1);
      }
      current_record.cube_color = GetColorType(token);
      draw.records.push_back(current_record);
      is_count = true;
    }
  }
  return draw;
}

[[nodiscard]] DrawVec ParseSubsets(const std::string& game_str) {
  DrawVec subsets;
  std::string buffer;
  int i = 0;
  while (i < game_str.size()) {
    if (';' == game_str[i]) {
      subsets.push_back(ParseDraw(buffer));
      buffer.clear();
      i += 2; /* skip the potential ' ' following the semicolon */
    } else {
      buffer += game_str[i];
      i++;
    }
  }
  subsets.push_back(ParseDraw(buffer)); /* collect the trailing draw */

  return subsets;
}

[[nodiscard]] GameVec LoadGames(const std::string& filename) {
  std::ifstream fhandle(filename);
  std::string line;
  GameVec games;
  while (std::getline(fhandle, line)) {
    Game new_game;
    std::sscanf(line.c_str(), "Game %d:", &new_game.id);
    std::size_t colon_idx = line.find(':');
    new_game.subsets = ParseSubsets(line.substr(colon_idx + 2));
    games.push_back(new_game);
  }
  return games;
}

[[nodiscard]] int IsPossibleGame(const Game& game,
                                 const RgbLimit& limits) noexcept {
  for (const Draw& draw : game.subsets) {
    for (const Record& record : draw.records) {
      switch (record.cube_color) {
        case Color::kRed:
          if (record.count > limits.red) {
            return false;
          }
          break;
        case Color::kGreen:
          if (record.count > limits.green) {
            return false;
          }
          break;
        case Color::kBlue:
          if (record.count > limits.blue) {
            return false;
          }
          break;
      }
    }
  }
  return true;
}

int main(int argc, char** argv) {
  if (argc != 2) {
    std::cerr << "error: missing puzzle input file" << std::endl;
    exit(EXIT_FAILURE);
  }

  GameVec games = LoadGames(argv[1]);
  RgbLimit limits;
  int id_sum = std::accumulate(
      games.cbegin(), games.cend(), 0, [&limits](int sum, const Game& g) {
        return sum + (IsPossibleGame(g, limits) ? g.id : 0);
      });
  std::cout << "Answer: " << id_sum << std::endl;

  exit(EXIT_SUCCESS);
}
