
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

struct NounVerb {
    int noun = 0;
    int verb = 0;
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

NounVerb FindNounAndVerb(const std::vector<int>& positions, int target) {
    const int kLowerBound = 0;
    const int kUpperBound = 99;
    for (int i = kLowerBound; i <= kUpperBound; ++i) {
        for (int j = kLowerBound; j <= kUpperBound; ++j) {
            std::vector<int> program = positions;
            program[1] = i;
            program[2] = j;

            ExecProgram(program);
            if (program[0] == target) {
                return {i, j};
            }
        }
    }
    return {-1, -1};
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

    const int kTarget = 19690720;
    NounVerb ans = FindNounAndVerb(positions, kTarget);

    std::cout << "Answer: " << (100 * ans.noun) + ans.verb << std::endl;

    exit(EXIT_SUCCESS);
}
