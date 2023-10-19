#include <fstream>
#include <iostream>
#include <limits>
#include <unordered_map>
#include <vector>

using BusId = int;

struct BusInfo {
  int target_time = 0;
  std::vector<BusId> bus_ids;
};

BusInfo LoadBusInfo(const std::string& infile) {
  std::fstream fhandle(infile);
  std::string line;

  std::getline(fhandle, line);
  BusInfo bus_info;
  bus_info.target_time = std::stoi(line);

  while (std::getline(fhandle, line, ',')) {
    if (line != "x") {
      bus_info.bus_ids.push_back(std::stoi(line));
    }
  }
  return bus_info;
}

std::pair<BusId, int> FindClosestBusId(
    const std::unordered_map<BusId, int>& crossing_times, int target_time) {
  BusId closest_id = -1;
  int min_delta = std::numeric_limits<int>::max();
  for (const auto& [id, timestamp] : crossing_times) {
    int delta = timestamp - target_time;
    if (delta < min_delta) {
      min_delta = delta;
      closest_id = id;
    }
  }
  return {closest_id, min_delta};
}

int Simulate(const BusInfo& info) {
  std::unordered_map<BusId, int> crossing_times;
  bool exit = false;
  while (!exit) {
    for (const BusId& id : info.bus_ids) {
      if (crossing_times[id] < info.target_time) {
        crossing_times[id] += id;
      }
    }

    exit = true;
    for (const auto& [id, timestamp] : crossing_times) {
      if (timestamp < info.target_time) {
        exit = false;
        break;
      }
    }
  }

  std::pair<BusId, int> closest =
      FindClosestBusId(crossing_times, info.target_time);

  return (closest.first * closest.second);
}

int main(int argc, char** argv) {
  if (argc != 2) {
    std::cerr << "error: missing puzzle input file" << std::endl;
    exit(EXIT_FAILURE);
  }

  BusInfo bus_info = LoadBusInfo(argv[1]);

  std::cout << "Answer: " << Simulate(bus_info) << std::endl;

  exit(EXIT_SUCCESS);
}
