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

void UpdateScannerPositions(std::vector<Layer>& firewall_layers) {
  for (Layer& layer : firewall_layers) {
    int next_scanner_pos = layer.scanner_pos + layer.scanner_dir;
    if (next_scanner_pos == layer.scanner_range) {
      layer.scanner_dir = -1;
    } else if (next_scanner_pos < 0) {
      layer.scanner_dir = 1;
    }
    layer.scanner_pos += layer.scanner_dir;
  }
}

int ComputeTripSeverity(std::vector<Layer>& firewall_layers) {
  int trip_severity = 0;
  int total_layers = firewall_layers.back().depth;
  int protected_idx = 0;
  for (int i = 0; i <= total_layers; ++i) {
    const Layer& protected_layer = firewall_layers[protected_idx];
    /* Check if packet is currently on a guarded layer. */
    if (protected_layer.depth == i) {
      /* Check if packet has collided with a scanner! */
      if (protected_layer.scanner_pos == 0) {
        trip_severity += protected_layer.depth * protected_layer.scanner_range;
      }
      protected_idx++;
    }
    UpdateScannerPositions(firewall_layers);
  }
  return trip_severity;
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

  std::cout << "Answer: " << ComputeTripSeverity(firewall_layers) << std::endl;

  exit(EXIT_SUCCESS);
}
