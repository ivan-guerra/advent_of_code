#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

using Layer = std::unordered_map<int, int>;

std::vector<Layer> ParseLayers(const std::string& pixels, int width,
                               int height) {
  const int kPixelsPerLayer = width * height;
  std::vector<Layer> layers;
  for (int i = 0; i < pixels.size(); i += kPixelsPerLayer) {
    Layer layer;
    for (int j = i; j < (kPixelsPerLayer + i); ++j) {
      layer[pixels[j] - '0']++;
    }
    layers.push_back(layer);
  }
  return layers;
}

Layer FindLayerWithLeastZeroes(const std::vector<Layer>& layers) {
  int min_zeroes = (layers[0].count(0) ? layers[0].at(0) : 0);
  int min_zeroes_layer_idx = 0;
  for (int i = 0; i < layers.size(); ++i) {
    if (!layers[i].count(0)) {
      min_zeroes = 0;
      min_zeroes_layer_idx = i;
    } else if (layers[i].at(0) < min_zeroes) {
      min_zeroes = layers[i].at(0);
      min_zeroes_layer_idx = i;
    }
  }
  return layers[min_zeroes_layer_idx];
}

int main(int argc, char** argv) {
  if (argc != 2) {
    std::cerr << "error: missing puzzle input file" << std::endl;
    exit(EXIT_FAILURE);
  }

  std::ifstream input(argv[1]);
  std::string pixels;
  std::getline(input, pixels);

  const int kWidth = 25;
  const int kHeight = 6;
  std::vector<Layer> layers = ParseLayers(pixels, kWidth, kHeight);

  Layer ans = FindLayerWithLeastZeroes(layers);

  std::cout << "Answer: " << (ans[1] * ans[2]) << std::endl;

  exit(EXIT_SUCCESS);
}
