#include <cstdlib>
#include <fstream>
#include <iostream>
#include <queue>
#include <string>
#include <unordered_map>

void CharMapInsert(std::unordered_map<char, int>& map, char c) {
  if (map.count(c))
    map[c]++;
  else
    map[c] = 1;
}

void CharMapRemove(std::unordered_map<char, int>& map, char c) {
  if (!map.count(c)) return;

  map[c]--;
  if (0 == map[c]) map.erase(map.find(c));
}

int GetNumCharsToMsgStart(const std::string& signal, int window_size) {
  std::queue<char> window;
  std::unordered_map<char, int> curr_chars;

  // Construct the first window of chars.
  int i = 0;
  while (i < window_size) {
    window.push(signal[i]);
    CharMapInsert(curr_chars, signal[i]);
    i++;
  }

  // Seems we found the window with all unique chars.
  if (curr_chars.size() == window_size) return window_size;

  for (int i = window_size; i < signal.size(); ++i) {
    // Remove the oldest char out of the current window.
    CharMapRemove(curr_chars, window.front());
    window.pop();

    // Slide the window by adding the latest character in the input stream.
    window.push(signal[i]);
    CharMapInsert(curr_chars, signal[i]);
    if (curr_chars.size() == window_size) return (i + 1);
  }
  return -1;
}

int main(int argc, char** argv) {
  if (argc != 2) {
    std::cerr << "usage: aoc INPUT_FILE" << std::endl;
    exit(EXIT_FAILURE);
  }

  // Load in the datastream.
  std::string signal;
  std::ifstream input(argv[1]);
  std::getline(input, signal);

  const int kStartSeqSize = 14;
  std::cout << "There are " << GetNumCharsToMsgStart(signal, kStartSeqSize)
            << " chars until beginning of message." << std::endl;

  exit(EXIT_SUCCESS);
}
