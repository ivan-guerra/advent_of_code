#include <cmath>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

using Position2D = std::pair<int, int>;

struct Direction {
    char orientation = '\0';
    int num_steps = 0;
};

std::vector<Direction> ParseDirections(const std::string& directions) {
    std::vector<Direction> sequence;
    std::istringstream iss(directions);
    std::vector<std::string> tokens;
    std::copy(std::istream_iterator<std::string>(iss),
              std::istream_iterator<std::string>(), std::back_inserter(tokens));

    for (const std::string& token : tokens) {
        Direction dir;
        dir.orientation = token.front();
        if (token.back() == ',') {
            std::string trimmed = token.substr(0, token.size() - 1);
            dir.num_steps = std::stoi(trimmed.substr(1));
        } else {
            dir.num_steps = std::stoi(token.substr(1));
        }
        sequence.push_back(dir);
    }

    return sequence;
}

Position2D GetFinalLocation(const std::vector<Direction>& sequence) {
    enum class CardinalDir {
        kNorth,
        kSouth,
        kEast,
        kWest,
    };

    Position2D curr_pos(0, 0);
    CardinalDir curr_dir = CardinalDir::kNorth;
    for (const Direction& d : sequence) {
        switch (curr_dir) {
            case CardinalDir::kNorth:
                curr_dir = (d.orientation == 'R') ? CardinalDir::kEast
                                                  : CardinalDir::kWest;
                curr_pos.first +=
                    (d.orientation == 'R') ? d.num_steps : -d.num_steps;
                break;
            case CardinalDir::kSouth:
                curr_dir = (d.orientation == 'R') ? CardinalDir::kWest
                                                  : CardinalDir::kEast;
                curr_pos.first +=
                    (d.orientation == 'R') ? -d.num_steps : d.num_steps;
                break;
            case CardinalDir::kEast:
                curr_dir = (d.orientation == 'R') ? CardinalDir::kSouth
                                                  : CardinalDir::kNorth;
                curr_pos.second +=
                    (d.orientation == 'R') ? -d.num_steps : d.num_steps;
                break;
            case CardinalDir::kWest:
                curr_dir = (d.orientation == 'R') ? CardinalDir::kNorth
                                                  : CardinalDir::kSouth;
                curr_pos.second +=
                    (d.orientation == 'R') ? d.num_steps : -d.num_steps;
                break;
        }
    }
    return curr_pos;
}

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "error: missing puzzle input file" << std::endl;
        exit(EXIT_FAILURE);
    }

    std::ifstream input(argv[1]);
    std::string directions;
    std::getline(input, directions);
    std::vector<Direction> sequence = ParseDirections(directions);

    Position2D final_pos = GetFinalLocation(sequence);
    int distance_to_hq = std::abs(final_pos.first) + std::abs(final_pos.second);
    std::cout << "Answer: " << distance_to_hq << std::endl;

    exit(EXIT_SUCCESS);
}
