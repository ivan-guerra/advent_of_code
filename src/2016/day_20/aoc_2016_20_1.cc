#include <algorithm>
#include <cstdint>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using IpRange = std::pair<uint32_t, uint32_t>;

IpRange ParseRange(const std::string& text) {
  int dash_index = text.find("-");
  uint32_t low = std::stoul(text.substr(0, dash_index));
  uint32_t high = std::stoul(text.substr(dash_index + 1));

  return IpRange(low, high);
}

int GetLowestValuedFreeIp(std::vector<IpRange>& blocked_ranges) {
  std::sort(
      blocked_ranges.begin(), blocked_ranges.end(),
      [](const IpRange& a, const IpRange& b) { return (a.first < b.first); });

  for (int i = 1; i < blocked_ranges.size(); ++i) {
    const IpRange& a = blocked_ranges[i - 1];
    const IpRange& b = blocked_ranges[i];
    if ((b.first > a.second) && ((b.first - a.second) > 1)) {
      return (a.second + 1);
    }
  }
  return -1; /* all IPs are blocked */
}

int main(int argc, char** argv) {
  if (argc != 2) {
    std::cerr << "error: missing puzzle input file" << std::endl;
    exit(EXIT_FAILURE);
  }

  std::ifstream input(argv[1]);
  std::string range;
  std::vector<IpRange> blocked_ranges;
  while (std::getline(input, range)) {
    blocked_ranges.push_back(ParseRange(range));
  }

  std::cout << "Answer: " << GetLowestValuedFreeIp(blocked_ranges) << std::endl;

  exit(EXIT_SUCCESS);
}
