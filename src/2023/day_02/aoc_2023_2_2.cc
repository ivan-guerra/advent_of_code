#include <cstdlib>
#include <fstream>
#include <iostream>
#include <limits>
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

[[nodiscard]] int GetMinPower(const Game& game) noexcept {
  int red_min = 0;
  int green_min = 0;
  int blue_min = 0;
  for (const Draw& draw : game.subsets) {
    for (const Record& record : draw.records) {
      switch (record.cube_color) {
        case Color::kRed:
          red_min = std::max(red_min, record.count);
          break;
        case Color::kGreen:
          green_min = std::max(green_min, record.count);
          break;
        case Color::kBlue:
          blue_min = std::max(blue_min, record.count);
          break;
      }
    }
  }
  return (red_min * green_min * blue_min);
}

int main(int argc, char** argv) {
  if (argc != 2) {
    std::cerr << "error: missing puzzle input file" << std::endl;
    exit(EXIT_FAILURE);
  }

  GameVec games = LoadGames(argv[1]);
  int sum_of_min_powers = std::accumulate(
      games.cbegin(), games.cend(), 0,
      [](int sum, const Game& g) { return (sum + GetMinPower(g)); });
  std::cout << "Answer: " << sum_of_min_powers << std::endl;

  exit(EXIT_SUCCESS);
}
