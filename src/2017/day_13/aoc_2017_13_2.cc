#include <cstdlib>
#include <fstream>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>
#include <vector>

struct Layer {
  int depth = 0;
  int scanner_range = 0;
  int scanner_pos = 0;
  int scanner_dir = 1;
};

Layer ParseLayer(const std::string& layer_info) {
  std::istringstream iss(layer_info);
  std::vector<std::string> tokens;
  std::copy(std::istream_iterator<std::string>(iss),
            std::istream_iterator<std::string>(), std::back_inserter(tokens));

  Layer layer = {
      .depth = std::stoi(tokens[0].substr(0, tokens[0].size() - 1)),
      .scanner_range = std::stoi(tokens[1]),
      .scanner_pos = 0,
  };
  return layer;
}

bool IsPacketDetected(std::vector<Layer> firewall_layers, int delay = 0) {
  for (const Layer& layer : firewall_layers) {
    if ((delay + layer.depth) % (2 * layer.scanner_range - 2) == 0) {
      return true;
    }
  }
  return false;
}

int ComputeMinDelayToAvoidDetection(const std::vector<Layer>& firewall_layers) {
  int delay = 1;
  while (IsPacketDetected(firewall_layers, delay)) {
    delay++;
  }
  return delay;
}

int main(int argc, char** argv) {
  if (argc != 2) {
    std::cerr << "error: missing puzzle input file" << std::endl;
    exit(EXIT_FAILURE);
  }

  std::ifstream input(argv[1]);
  std::string layer_info;
  std::vector<Layer> firewall_layers;
  while (std::getline(input, layer_info)) {
    firewall_layers.push_back(ParseLayer(layer_info));
  }

  std::cout << "Answer: " << ComputeMinDelayToAvoidDetection(firewall_layers)
            << std::endl;

  exit(EXIT_SUCCESS);
}
