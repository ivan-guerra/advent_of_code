#include <deque>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

std::vector<uint64_t> LoadNums(const std::string& infile) {
  std::ifstream fhandle(infile);
  std::string line;
  std::vector<uint64_t> nums;
  while (std::getline(fhandle, line, ',')) {
    nums.push_back(std::stoi(line));
  }
  return nums;
}

int FindTargetCallout(std::vector<uint64_t>& nums, int target) {
  std::unordered_map<int, std::deque<int>> history;
  int turn = 1;
  for (const uint64_t& i : nums) {
    history[i].push_back(turn);
    while (history[i].size() > 2) {
      history[i].pop_front();
    }
    turn++;
  }

  while (turn <= target) {
    if (history[nums.back()].size() < 2) {
      nums.push_back(0);

      history[0].push_back(turn);
      while (history[0].size() > 2) {
        history[0].pop_front();
      }
    } else {
      int front = history[nums.back()].front();
      int new_num = front;
      history[nums.back()].pop_front();

      new_num = history[nums.back()].front() - new_num;
      history[nums.back()].push_front(front);

      nums.push_back(new_num);

      history[new_num].push_back(turn);
      while (history[new_num].size() > 2) {
        history[new_num].pop_front();
      }
    }
    turn++;
  }
  return nums.back();
}

int main(int argc, char** argv) {
  if (argc != 2) {
    std::cerr << "error: missing puzzle input file" << std::endl;
    exit(EXIT_FAILURE);
  }

  const int kTarget = 30000000;
  std::vector<uint64_t> nums = LoadNums(argv[1]);
  std::cout << "Answer: " << FindTargetCallout(nums, kTarget) << std::endl;

  exit(EXIT_SUCCESS);
}
