#include <unistd.h>

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>
#include <vector>

using LightMatrix = std::vector<std::vector<char>>;

struct Position {
  int x = 0;
  int y = 0;
};

struct Velocity {
  int x = 0;
  int y = 0;
};

struct Light {
  Position pos;
  Velocity velocity;
};

Light ParseLight(const std::string& text) {
  std::istringstream iss(text);
  std::vector<std::string> words;
  std::copy(std::istream_iterator<std::string>(iss),
            std::istream_iterator<std::string>(), std::back_inserter(words));

  Light light;
  light.pos.y = std::stoi(words[1].substr(0, words[1].size() - 1));
  light.pos.x = std::stoi(words[2]);
  light.velocity.y = std::stoi(words[5].substr(0, words[5].size() - 1));
  light.velocity.x = std::stoi(words[6]);

  return light;
}

bool PixelIsOn(const std::vector<Light>& lights, int x, int y) {
  for (const Light& light : lights) {
    if ((light.pos.x == x) && (light.pos.y == y)) {
      return true;
    }
  }
  return false;
}

void DrawDisplay(const LightMatrix& display, const std::vector<Light>& lights) {
  std::cout << std::endl;
  for (int i = 0; i < display.size(); ++i) {
    for (int j = 0; j < display[i].size(); ++j) {
      if (PixelIsOn(lights, i, j)) {
        std::cout << '#';
      } else {
        std::cout << '.';
      }
    }
    std::cout << std::endl;
  }
  std::cout << std::endl;
}

void UpdateLights(std::vector<Light>& lights) {
  for (Light& light : lights) {
    light.pos.x += light.velocity.x;
    light.pos.y += light.velocity.y;
  }
}

int DistanceSquared(const Light& a, const Light& b) {
  int x2 = (b.pos.x - a.pos.x) * (b.pos.x - a.pos.x);
  int y2 = (b.pos.y - a.pos.y) * (b.pos.y - a.pos.y);

  return (x2 + y2);
}

std::vector<Light> LightsWithinRadius(const Light& ref,
                                      const std::vector<Light>& lights,
                                      int radius) {
  std::vector<Light> neighbors;
  int r2 = radius * radius;
  for (const Light& light : lights) {
    int dist2 = DistanceSquared(ref, light);
    if (dist2 <= r2) {
      neighbors.push_back(light);
    }
  }
  return neighbors;
}

void RunSimulation(std::vector<Light>& lights) {
  const int kNumPixels = 300;
  LightMatrix display(kNumPixels, std::vector<char>(kNumPixels, '#'));

  const int kSolutionSecond = 10681;
  int second = 0;
  while (true) {
    if (second == kSolutionSecond) {
      DrawDisplay(display, lights);
      return;
    }

    /* Enable the code below to preview potential solutions when searching
     * for your solution second. */
    /*
    int search_radius = 200;
    std::vector<Light> neighbors =
        LightsWithinRadius(lights[0], lights, search_radius);
    if (neighbors.size() > (lights.size() * 0.75)) {
        std::cout << "potential solution at second " << second << std::endl;
        DrawDisplay(display, lights);
        sleep(1);
    }
    */

    UpdateLights(lights);
    second++;
  }
}

int main(int argc, char** argv) {
  if (argc != 2) {
    std::cerr << "error: missing puzzle input file" << std::endl;
    exit(EXIT_FAILURE);
  }

  std::ifstream input(argv[1]);
  std::string line;
  std::vector<Light> lights;
  while (std::getline(input, line)) {
    lights.push_back(ParseLight(line));
  }

  RunSimulation(lights);

  exit(EXIT_SUCCESS);
}
