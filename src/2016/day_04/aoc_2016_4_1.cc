#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

struct Room {
    std::unordered_map<char, int> char_frequencies;
    int id = 0;
    std::string checksum;
};

bool IsRealRoom(const Room& room) {
    std::vector<std::pair<char, int>> char_freqs(room.char_frequencies.begin(),
                                                 room.char_frequencies.end());

    std::sort(char_freqs.begin(), char_freqs.end(),
              [](const std::pair<char, int>& a, const std::pair<char, int>& b) {
                  if (a.second != b.second) {
                      return (a.second > b.second);
                  }
                  return (a.first < b.first);
              });

    if (char_freqs.size() < room.checksum.size()) {
        return false;
    }

    for (int i = 0; i < room.checksum.size(); ++i) {
        if (room.checksum[i] != char_freqs[i].first) {
            return false;
        }
    }
    return true;
}

Room ParseRoom(const std::string& room_str) {
    Room room;
    int i = 0;

    /* Parse the encrypted name's character frequencies. */
    while (!std::isdigit(room_str[i])) {
        if ('-' != room_str[i]) {
            room.char_frequencies[room_str[i]]++;
        }
        i++;
    }

    /* Parse sector ID. */
    while (std::isdigit(room_str[i])) {
        room.id = (room.id * 10) + (room_str[i] - '0');
        i++;
    }

    /* Parse checksum. */
    i++;  // Skip opening checksum '['.
    while (room_str[i] != ']') {
        room.checksum += room_str[i];
        i++;
    }
    return room;
}

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "error: missing puzzle input file" << std::endl;
        exit(EXIT_FAILURE);
    }

    std::ifstream input(argv[1]);
    std::string room;
    std::vector<Room> rooms;
    while (std::getline(input, room)) {
        rooms.push_back(ParseRoom(room));
    }

    int sector_sum = 0;
    for (const Room& r : rooms) {
        if (IsRealRoom(r)) {
            sector_sum += r.id;
        }
    }
    std::cout << "Answer: " << sector_sum << std::endl;

    exit(EXIT_SUCCESS);
}
