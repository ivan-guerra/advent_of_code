#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

enum OpCodes {
    kAdd = 1,
    kMultiply = 2,
    kHalt = 99,
};

std::vector<int> ParsePositions(const std::string& input) {
    std::stringstream ss(input);
    std::vector<int> positions;
    std::string token;
    while (std::getline(ss, token, ',')) {
        positions.push_back(std::stoi(token));
    }
    return positions;
}

void ExecProgram(std::vector<int>& positions) {
    const int kInstrSize = 4;
    const int kArg1Offset = 1;
    const int kArg2Offset = 2;
    const int kOutOffset = 3;

    int i = 0;
    while (positions[i] != OpCodes::kHalt) {
        switch (positions[i]) {
            case OpCodes::kAdd:
                positions[positions[i + kOutOffset]] =
                    positions[positions[i + kArg1Offset]] +
                    positions[positions[i + kArg2Offset]];
                break;
            case OpCodes::kMultiply:
                positions[positions[i + kOutOffset]] =
                    positions[positions[i + kArg1Offset]] *
                    positions[positions[i + kArg2Offset]];
                break;
        }
        i += kInstrSize;
    }
}

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "error: missing puzzle input file" << std::endl;
        exit(EXIT_FAILURE);
    }

    std::ifstream input(argv[1]);
    std::string line;
    std::getline(input, line);
    std::vector<int> positions = ParsePositions(line);

    /* Hardcoding position changes given in problem statement. */
    positions[1] = 12;
    positions[2] = 2;

    ExecProgram(positions);

    std::cout << "Answer: " << positions[0] << std::endl;

    exit(EXIT_SUCCESS);
}
