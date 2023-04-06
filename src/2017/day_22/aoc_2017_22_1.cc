#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_set>
#include <vector>

struct Coordinate2D;
struct Coordinate2DHash;

using Matrix = std::vector<std::string>;
using Infections = std::unordered_set<Coordinate2D, Coordinate2DHash>;

enum class Direction {
    kUp,
    kDown,
    kLeft,
    kRight,
};

struct Coordinate2D {
    int x = 0;
    int y = 0;

    Coordinate2D(int x_, int y_) : x(x_), y(y_) {}
    friend bool operator==(const Coordinate2D& a, const Coordinate2D& b) {
        return ((a.x == b.x) && (a.y == b.y));
    }
};

struct Coordinate2DHash {
    std::size_t operator()(const Coordinate2D& c) const {
        std::string coord_str = std::to_string(c.x) + "," + std::to_string(c.y);
        return std::hash<std::string>{}(coord_str);
    }
};

void InitInfections(Infections& infections, const Matrix& init_map) {
    for (int i = 0; i < init_map.size(); ++i) {
        for (int j = 0; j < init_map[0].size(); ++j) {
            if (init_map[i][j] == '#') {
                infections.emplace(i, j);
            }
        }
    }
}

void TurnRight(Direction& dir) {
    switch (dir) {
        case Direction::kUp:
            dir = Direction::kRight;
            break;
        case Direction::kDown:
            dir = Direction::kLeft;
            break;
        case Direction::kLeft:
            dir = Direction::kUp;
            break;
        case Direction::kRight:
            dir = Direction::kDown;
            break;
    }
}

void TurnLeft(Direction& dir) {
    switch (dir) {
        case Direction::kUp:
            dir = Direction::kLeft;
            break;
        case Direction::kDown:
            dir = Direction::kRight;
            break;
        case Direction::kLeft:
            dir = Direction::kDown;
            break;
        case Direction::kRight:
            dir = Direction::kUp;
            break;
    }
}

int ComputeNewInfections(Infections& infections, const Coordinate2D& origin,
                         int num_bursts) {
    int num_new_infections = 0;
    Direction curr_direction = Direction::kUp;
    Coordinate2D curr_pos = origin;
    while (num_bursts--) {
        if (infections.count(curr_pos)) {
            TurnRight(curr_direction);
        } else {
            TurnLeft(curr_direction);
        }

        if (!infections.count(curr_pos)) {
            num_new_infections++;
            infections.insert(curr_pos);
        } else {
            infections.erase(curr_pos);
        }
        switch (curr_direction) {
            case Direction::kUp:
                curr_pos.x--;
                break;
            case Direction::kDown:
                curr_pos.x++;
                break;
            case Direction::kLeft:
                curr_pos.y--;
                break;
            case Direction::kRight:
                curr_pos.y++;
                break;
        }
    }
    return num_new_infections;
}

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "error: missing puzzle input file" << std::endl;
        exit(EXIT_FAILURE);
    }

    std::ifstream input(argv[1]);
    std::string line;
    Matrix init_map;
    while (std::getline(input, line)) {
        init_map.push_back(line);
    }

    Coordinate2D origin(init_map.size() / 2, init_map[0].size() / 2);
    Infections infections;
    InitInfections(infections, init_map);

    const int kNumBursts = 10000;
    std::cout << "Answer: "
              << ComputeNewInfections(infections, origin, kNumBursts)
              << std::endl;

    exit(EXIT_SUCCESS);
}
