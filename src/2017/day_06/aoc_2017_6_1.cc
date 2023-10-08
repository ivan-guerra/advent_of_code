#include <cstdlib>
#include <fstream>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>
#include <unordered_set>
#include <vector>

using Blocks = std::vector<int>;

Blocks ParseBlocks(const std::string& config) {
  std::istringstream iss(config);
  std::vector<std::string> tokens;
  std::copy(std::istream_iterator<std::string>(iss),
            std::istream_iterator<std::string>(), std::back_inserter(tokens));

  Blocks blocks;
  for (const std::string& token : tokens) {
    blocks.push_back(std::stoi(token));
  }
  return blocks;
}

std::string GenerateConfigKey(const Blocks& blocks) {
  std::string key;
  for (const int& i : blocks) {
    key += std::to_string(i) + ",";
  }
  return key;
}

int GetLargestMemBank(const Blocks& blocks) {
  int max_val = blocks[0];
  int max_idx = 0;
  for (int i = 1; i < blocks.size(); ++i) {
    if (blocks[i] > max_val) {
      max_val = blocks[i];
      max_idx = i;
    }
  }
  return max_idx;
}

int ComputeNumRedistributionCycles(Blocks& blocks) {
  int num_cycles = 0;
  std::string config_key;
  std::unordered_set<std::string> prev_configs;
  while (!prev_configs.count(config_key)) {
    prev_configs.insert(config_key);

    /* Locate and clear the largest memory bank. */
    int largest_membank = GetLargestMemBank(blocks);
    int blocks_to_distribute = blocks[largest_membank];
    blocks[largest_membank] = 0;

    /* Perform the redistribution. */
    int i = (largest_membank + 1) % blocks.size();
    while (blocks_to_distribute--) {
      blocks[i]++;
      i = (i + 1) % blocks.size();
    }

    /* Compute the key for the current configuration. */
    config_key = GenerateConfigKey(blocks);

    num_cycles++;
  }
  return num_cycles;
}

int main(int argc, char** argv) {
  if (argc != 2) {
    std::cerr << "error: missing puzzle input file" << std::endl;
    exit(EXIT_FAILURE);
  }

  std::ifstream input(argv[1]);
  std::string config;
  std::getline(input, config);

  Blocks blocks = ParseBlocks(config);

  std::cout << "Answer: " << ComputeNumRedistributionCycles(blocks)
            << std::endl;

  exit(EXIT_SUCCESS);
}
