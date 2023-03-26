#include <cmath>
#include <cstdlib>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <queue>
#include <string>
#include <unordered_set>
#include <vector>

using Matrix = std::vector<std::vector<int>>;

struct Cell {
    int x = 0;
    int y = 0;
    Cell(int x_, int y_) : x(x_), y(y_) {}

    friend bool operator==(const Cell& a, const Cell& b) {
        return ((a.x == b.x) && (a.y == b.y));
    }
};

struct CellHash {
    std::size_t operator()(const Cell& cell) const {
        std::string hash_str =
            std::to_string(cell.x) + "," + std::to_string(cell.y);
        return std::hash<std::string>{}(hash_str);
    }
};

Matrix GenerateMemMatrix(int target) {
    int dim = std::ceil(std::sqrt(target));
    Matrix ret(dim, std::vector<int>(dim, 0));

    std::vector<int> buffer(dim * dim, 0);
    for (int i = 0; i < buffer.size(); ++i) {
        buffer[i] = i + 1;
    }

    int top = 0;
    int bottom = ret.size() - 1;
    int left = 0;
    int right = ret[0].size() - 1;
    int idx = buffer.size() - 1;
    while (true) {
        if (left > right) {
            break;
        }

        /* Bottom Row */
        for (int i = right; i >= left; --i) {
            ret[bottom][i] = buffer[idx--];
        }
        bottom--;

        if (top > bottom) {
            break;
        }

        /* Right Col */
        for (int i = bottom; i >= top; --i) {
            ret[i][left] = buffer[idx--];
        }
        left++;

        if (left > right) {
            break;
        }

        /* Bottom Row */
        for (int i = left; i <= right; ++i) {
            ret[top][i] = buffer[idx--];
        }
        top++;

        if (top > bottom) {
            break;
        }

        /* Left Col */
        for (int i = top; i <= bottom; ++i) {
            ret[i][right] = buffer[idx--];
        }
        right--;
    }
    return ret;
}

Cell GetMemCellByValue(const Matrix& mem, int value) {
    for (int i = 0; i < mem.size(); ++i) {
        for (int j = 0; j < mem[0].size(); ++j) {
            if (mem[i][j] == value) {
                return Cell(i, j);
            }
        }
    }
    return Cell(-1, -1); /* unknown value */
}

std::vector<Cell> GetCellNeighbors(const Matrix& mem, const Cell& cell) {
    enum Direction { kUp, kDown, kLeft, kRight };
    static const std::vector<Direction> kDirs = {
        Direction::kUp,
        Direction::kDown,
        Direction::kLeft,
        Direction::kRight,
    };

    std::vector<Cell> neighbors;
    for (const Direction& dir : kDirs) {
        int row = cell.x;
        int col = cell.y;
        switch (dir) {
            case Direction::kUp:
                row--;
                break;
            case Direction::kDown:
                row++;
                break;
            case Direction::kLeft:
                col--;
                break;
            case Direction::kRight:
                col++;
                break;
        }
        if ((row >= 0) && (row < mem.size()) && (col >= 0) &&
            (col < mem[0].size())) {
            neighbors.emplace_back(row, col);
        }
    }
    return neighbors;
}

int MinStepsToAccessPort(const Matrix& mem, const Cell& src, const Cell& dst) {
    std::queue<Cell> nodes;
    nodes.push(src);
    std::unordered_set<Cell, CellHash> visited;
    int dist = 0;
    while (!nodes.empty()) {
        int num_nodes = nodes.size();
        for (int i = 0; i < num_nodes; ++i) {
            Cell curr = nodes.front();
            nodes.pop();

            if (visited.count(curr)) {
                continue;
            }
            if (curr == dst) {
                return dist;
            }

            visited.insert(curr);
            std::vector<Cell> neighbors = GetCellNeighbors(mem, curr);
            for (const Cell& neighbor : neighbors) {
                if (!visited.count(neighbor)) {
                    nodes.push(neighbor);
                }
            }
        }
        dist++;
    }
    return -1; /* dst cell is unreachable from src cell */
}

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "error: missing puzzle input file" << std::endl;
        exit(EXIT_FAILURE);
    }

    std::ifstream input(argv[1]);
    std::string target;
    std::getline(input, target);

    Matrix mem = GenerateMemMatrix(std::stoi(target));
    Cell src = GetMemCellByValue(mem, std::stoi(target));
    Cell dst = GetMemCellByValue(mem, 1);

    std::cout << "Answer: " << MinStepsToAccessPort(mem, src, dst) << std::endl;

    exit(EXIT_SUCCESS);
}
