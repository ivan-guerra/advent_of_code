#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using PacketMap = std::vector<std::string>;

enum Direction {
    kNorth,
    kEast,
    kSouth,
    kWest,
};

struct Coordinate2D {
    int x = 0;
    int y = 0;
};

void UpdateDirection(const PacketMap& map, const Coordinate2D& pos,
                     Direction& dir) {
    if ((dir == Direction::kNorth) || (dir == Direction::kSouth)) {
        int left = pos.y - 1;
        if ((left >= 0) && (map[pos.x][left] != ' ')) {
            dir = Direction::kWest;
        } else {
            dir = Direction::kEast;
        }
    } else {
        int up = pos.x - 1;
        if ((up >= 0) && (map[up][pos.y] != ' ')) {
            dir = Direction::kNorth;
        } else {
            dir = Direction::kSouth;
        }
    }
}

int CountSteps(const PacketMap& map) {
    /* Find the starting position. */
    Coordinate2D pos = {
        .x = 0,
        .y = static_cast<int>(map[0].find("|")),
    };
    int num_steps = 0;
    Direction dir = Direction::kSouth;
    while (true) {
        const char& symbol = map[pos.x][pos.y];
        if (symbol == '+') {
            /* Change direction. */
            UpdateDirection(map, pos, dir);
        } else if ((symbol == '|') || (symbol == '-')) {
            /* No change in direction. */
        } else if (' ' == symbol) {
            /* Hit the end of map. */
            break;
        }

        switch (dir) {
            case kNorth:
                pos.x--;
                break;
            case kEast:
                pos.y++;
                break;
            case kSouth:
                pos.x++;
                break;
            case kWest:
                pos.y--;
                break;
        }
        num_steps++;
    }
    return num_steps;
}

void SquarePacketMap(PacketMap& map) {
    std::size_t max_line = 0;
    for (const std::string& s : map) {
        max_line = std::max(max_line, s.size());
    }

    for (std::string& s : map) {
        while (s.size() < max_line) {
            s += ' ';
        }
    }
}

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "error: missing puzzle input file" << std::endl;
        exit(EXIT_FAILURE);
    }

    std::ifstream input(argv[1]);
    std::string line;
    PacketMap map;
    while (std::getline(input, line)) {
        map.push_back(line);
    }
    SquarePacketMap(map);

    std::cout << "Answer: " << CountSteps(map) << std::endl;

    exit(EXIT_SUCCESS);
}
