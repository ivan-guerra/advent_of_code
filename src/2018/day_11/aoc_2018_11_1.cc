#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using FuelCells = std::vector<std::vector<int>>;
using Coord2D = std::pair<int, int>;

int ComputePowerLevel(int serial, int x, int y) {
    int rack_id = x + 10;
    int power_level = rack_id;

    power_level *= y;
    power_level += serial;
    power_level *= rack_id;
    power_level = (power_level / 100) % 10;
    power_level -= 5;

    return power_level;
}

FuelCells InitCells(int serial, int rows, int cols) {
    FuelCells cells(rows, std::vector<int>(cols, 0));
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            cells[i][j] = ComputePowerLevel(serial, j, i);
        }
    }
    return cells;
}

Coord2D MaxPowerLevel(const FuelCells& cells, int square_dim) {
    int max_power = 0;
    Coord2D max_power_coord;
    for (int i = 0; (cells.size() - i) >= square_dim; ++i) {
        for (int j = 0; (cells.size() - j) >= square_dim; ++j) {
            int curr_power = 0;
            for (int k = i; k < (i + square_dim); ++k) {
                for (int q = j; q < (j + square_dim); ++q) {
                    curr_power += cells[k][q];
                }
            }
            if (curr_power > max_power) {
                max_power = curr_power;
                max_power_coord = std::make_pair(j, i);
            }
        }
    }
    return max_power_coord;
}

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "error: missing puzzle input file" << std::endl;
        exit(EXIT_FAILURE);
    }

    std::ifstream input(argv[1]);
    std::string line;
    std::getline(input, line);
    int serial = std::stoi(line);

    const int kDim = 300;
    FuelCells cells = InitCells(serial, kDim, kDim);

    const int kSquareDim = 3;
    Coord2D max_power = MaxPowerLevel(cells, kSquareDim);

    std::cout << "Answer: " << max_power.first << ", " << max_power.second
              << std::endl;

    exit(EXIT_SUCCESS);
}
