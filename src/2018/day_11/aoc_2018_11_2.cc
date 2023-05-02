#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using FuelCells = std::vector<std::vector<int>>;

struct Solution {
    int x = 0;
    int y = 0;
    int square_dim = 0;
    int power_level = 0;
};

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

Solution MaxPowerLevel(const FuelCells& cells, int square_dim) {
    int max_power = 0;
    Solution soln;
    soln.square_dim = square_dim;
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
                soln.x = j;
                soln.y = i;
                soln.power_level = max_power;
            }
        }
    }
    return soln;
}

Solution MaxGlobalPowerLevel(const FuelCells& cells) {
    Solution max_soln;
    for (int square_dim = 1; square_dim <= cells.size(); ++square_dim) {
        Solution curr_soln = MaxPowerLevel(cells, square_dim);
        if (curr_soln.power_level > max_soln.power_level) {
            max_soln = curr_soln;
        }
    }
    return max_soln;
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
    Solution global_max_power = MaxGlobalPowerLevel(cells);

    std::cout << "Answer: " << global_max_power.x << ", " << global_max_power.y
              << ", " << global_max_power.square_dim << std::endl;

    exit(EXIT_SUCCESS);
}
