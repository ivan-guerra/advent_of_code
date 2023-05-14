#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

enum class Acre {
    kOpen,
    kTree,
    kLumberyard,
};

using Woods = std::vector<std::vector<Acre>>;

Woods ParseInitState(const std::string& filename) {
    std::ifstream input(filename);

    Woods woods;
    std::string line;
    while (std::getline(input, line)) {
        std::vector<Acre> row;
        for (const char& c : line) {
            switch (c) {
                case '.':
                    row.push_back(Acre::kOpen);
                    break;
                case '#':
                    row.push_back(Acre::kLumberyard);
                    break;
                case '|':
                    row.push_back(Acre::kTree);
                    break;
                default:
                    std::cerr << "error: unknown land marker!" << std::endl;
                    break;
            }
        }
        woods.push_back(row);
    }
    return woods;
}

bool ApplyOpenRule(const Woods& woods, int i, int j) {
    static const std::vector<std::pair<int, int>> kMoves = {
        {0, 1}, {1, 0}, {-1, 0}, {0, -1}, {1, 1}, {-1, -1}, {1, -1}, {-1, 1}};
    static const int kThreshNumTrees = 3;

    int num_trees = 0;
    for (const auto& move : kMoves) {
        int row = i + move.first;
        int col = j + move.second;
        if ((row >= 0) && (row < woods.size()) && (col >= 0) &&
            (col < woods[i].size()) && (woods[row][col] == Acre::kTree)) {
            num_trees++;
        }
    }
    return (num_trees >= kThreshNumTrees);
}

bool ApplyTreeRule(const Woods& woods, int i, int j) {
    static const std::vector<std::pair<int, int>> kMoves = {
        {0, 1}, {1, 0}, {-1, 0}, {0, -1}, {1, 1}, {-1, -1}, {1, -1}, {-1, 1}};
    static const int kThreshLumberyards = 3;

    int num_lumberyards = 0;
    for (const auto& move : kMoves) {
        int row = i + move.first;
        int col = j + move.second;
        if ((row >= 0) && (row < woods.size()) && (col >= 0) &&
            (col < woods[i].size()) && (woods[row][col] == Acre::kLumberyard)) {
            num_lumberyards++;
        }
    }
    return (num_lumberyards >= kThreshLumberyards);
}

bool ApplyLumberyardRule(const Woods& woods, int i, int j) {
    static const std::vector<std::pair<int, int>> kMoves = {
        {0, 1}, {1, 0}, {-1, 0}, {0, -1}, {1, 1}, {-1, -1}, {1, -1}, {-1, 1}};
    static const int kThreshLumberyards = 1;
    static const int kThreshNumTrees = 1;

    int num_lumberyards = 0;
    int num_trees = 0;
    for (const auto& move : kMoves) {
        int row = i + move.first;
        int col = j + move.second;
        if ((row >= 0) && (row < woods.size()) && (col >= 0) &&
            (col < woods[i].size())) {
            if (woods[row][col] == Acre::kTree) {
                num_trees++;
            } else if (woods[row][col] == Acre::kLumberyard) {
                num_lumberyards++;
            }
        }
    }
    return ((num_lumberyards >= kThreshLumberyards) &&
            (num_trees >= kThreshNumTrees));
}

void RunSimulation(Woods& state, int num_minutes) {
    while (num_minutes--) {
        Woods reference = state;
        for (int i = 0; i < reference.size(); ++i) {
            for (int j = 0; j < reference[i].size(); ++j) {
                switch (reference[i][j]) {
                    case Acre::kOpen:
                        if (ApplyOpenRule(reference, i, j)) {
                            state[i][j] = Acre::kTree;
                        }
                        break;
                    case Acre::kTree:
                        if (ApplyTreeRule(reference, i, j)) {
                            state[i][j] = Acre::kLumberyard;
                        }
                        break;
                    case Acre::kLumberyard:
                        if (!ApplyLumberyardRule(reference, i, j)) {
                            state[i][j] = Acre::kOpen;
                        }
                        break;
                }
            }
        }
    }
}

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "error: missing puzzle input file" << std::endl;
        exit(EXIT_FAILURE);
    }

    Woods woods = ParseInitState(argv[1]);

    const int kNumMinutes = 10;
    RunSimulation(woods, kNumMinutes);

    int num_wooded_acres = 0;
    int num_lumberyards = 0;
    for (const auto& row : woods) {
        for (const Acre& acre : row) {
            if (acre == Acre::kLumberyard) {
                num_lumberyards++;
            } else if (acre == Acre::kTree) {
                num_wooded_acres++;
            }
        }
    }
    std::cout << "Answer: " << (num_wooded_acres * num_lumberyards)
              << std::endl;

    exit(EXIT_SUCCESS);
}
