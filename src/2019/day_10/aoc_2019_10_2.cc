#include <cmath>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_set>
#include <vector>

using AsteroidMap = std::vector<std::string>;

int FindBestStation(const AsteroidMap& asteroids) {
  std::size_t width = asteroids[0].size();
  std::size_t height = asteroids.size();
  int best = -1;
  std::unordered_set<double> angles;
  for (int i = 0; i < height; ++i) {
    for (int j = 0; j < width; ++j) {
      if (asteroids[i][j] != '#') {
        continue;
      }

      angles.clear();
      for (int k = 0; k < height; ++k) {
        for (int l = 0; l < width; ++l) {
          if (asteroids[k][l] == '#') {
            angles.insert(std::atan2(k - i, l - j));
          }
        }
      }
      best = std::max(best, static_cast<int>(angles.size()));
    }
  }
  return best;
}

int main(int argc, char** argv) {
  if (argc != 2) {
    std::cerr << "error: missing puzzle input file" << std::endl;
    exit(EXIT_FAILURE);
  }

  std::ifstream input(argv[1]);
  std::string line;
  AsteroidMap asteroid_map;
  while (std::getline(input, line)) {
    asteroid_map.push_back(line);
  }

  std::cout << "Answer: " << FindBestStation(asteroid_map) << std::endl;

  exit(EXIT_SUCCESS);
}
