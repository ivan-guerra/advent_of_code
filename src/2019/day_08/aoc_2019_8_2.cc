#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

enum Color {
  kBlack = 0,
  kWhite = 1,
  kTransparent = 2,
};

using Layer = std::vector<std::vector<Color>>;

std::vector<Layer> ParseLayers(const std::string& pixels, int width,
                               int height) {
  const int kPixelsPerLayer = width * height;
  std::vector<Layer> layers;
  for (int i = 0; i < pixels.size(); i += kPixelsPerLayer) {
    Layer layer(height, std::vector<Color>(width, Color::kTransparent));
    for (int j = 0; j < height; ++j) {
      for (int k = 0; k < width; ++k) {
        int pixel = pixels[j * width + k + i] - '0';
        switch (pixel) {
          case Color::kBlack:
            layer[j][k] = Color::kBlack;
            break;
          case Color::kWhite:
            layer[j][k] = Color::kWhite;
            break;
          case Color::kTransparent:
            layer[j][k] = Color::kTransparent;
            break;
        }
      }
    }
    layers.push_back(layer);
  }
  return layers;
}

Layer GenerateOutputImage(const std::vector<Layer>& layers, int width,
                          int height) {
  Layer output(height, std::vector<Color>(width, Color::kTransparent));
  for (int i = 0; i < height; ++i) {
    for (int j = 0; j < width; ++j) {
      for (const Layer& layer : layers) {
        if (layer[i][j] != Color::kTransparent) {
          output[i][j] = layer[i][j];
          break;
        }
      }
    }
  }
  return output;
}

void PrintImage(const Layer& image) {
  for (const auto& row : image) {
    for (const Color& pixel : row) {
      if (pixel == Color::kWhite) {
        std::cout << '#' << ' ';
      } else if (pixel == Color::kBlack) {
        std::cout << ' ' << ' ';
      }
    }
    std::cout << std::endl;
  }
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
  Layer image = GenerateOutputImage(layers, kWidth, kHeight);
  PrintImage(image);

  exit(EXIT_SUCCESS);
}
