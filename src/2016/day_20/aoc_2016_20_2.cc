#include <algorithm>
#include <cstdint>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using IpRange = std::pair<uint64_t, uint64_t>;

IpRange ParseRange(const std::string& text) {
  int dash_index = text.find("-");
  uint64_t low = std::stoull(text.substr(0, dash_index));
  uint64_t high = std::stoull(text.substr(dash_index + 1));

  return IpRange(low, high);
}

uint64_t GetNumUnblockedIps(std::vector<IpRange>& blocked_ranges,
                            uint64_t total_ips) {
  std::sort(
      blocked_ranges.begin(), blocked_ranges.end(),
      [](const IpRange& a, const IpRange& b) { return (a.first < b.first); });

  uint64_t blocked_ips = 0;
  uint64_t min = blocked_ranges.front().first;
  uint64_t max = blocked_ranges.front().second;
  for (const IpRange& r : blocked_ranges) {
    if (r.first > (max + 1)) {
      blocked_ips += max - min + 1;
      min = r.first;
      max = r.second;
    } else {
      max = std::max(max, r.second);
    }
  }
  return (total_ips - blocked_ips - (max - min + 1));
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

  std::cout << "Answer: " << GetNumUnblockedIps(blocked_ranges, 4294967296ULL)
            << std::endl;

  exit(EXIT_SUCCESS);
}
