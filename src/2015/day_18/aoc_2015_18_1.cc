#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using LightMatrix = std::vector<std::string>;

void UpdateLight(LightMatrix& lights, int i, int j) {
    int num_lights_on = 0;

    /* Count the number of lights on in each of the eight directions. */
    for (int k = 0; k < 8; ++k) {
        int row = i;
        int col = j;
        switch (k) {
            case 0:
                row++;
                break;
            case 1:
                row--;
                break;
            case 2:
                col++;
                break;
            case 3:
                col--;
                break;
            case 4:
                row++;
                col++;
                break;
            case 5:
                row--;
                col--;
                break;
            case 6:
                row++;
                col--;
                break;
            case 7:
                row--;
                col++;
                break;
        }
        if ((row >= 0) && (row < lights.size()) && (col >= 0) &&
            (col < lights[0].size())) {
            if ((lights[row][col] == '#') || (lights[row][col] == '-')) {
                num_lights_on++;
            }
        }
    }

    /* Apply the rules based on the number of neighboring lights that were on.
     */
    if (lights[i][j] == '#') {
        if ((num_lights_on < 2) || (num_lights_on > 3)) {
            lights[i][j] = '-';
        }
    } else {
        if (num_lights_on == 3) {
            lights[i][j] = '+';
        }
    }
}

void UpdateLights(LightMatrix& lights) {
    for (int i = 0; i < lights.size(); ++i) {
        for (int j = 0; j < lights[0].size(); ++j) {
            UpdateLight(lights, i, j);
        }
    }

    for (int i = 0; i < lights.size(); ++i) {
        for (int j = 0; j < lights[0].size(); ++j) {
            if (lights[i][j] == '+') {
                lights[i][j] = '#';
            }
            if (lights[i][j] == '-') {
                lights[i][j] = '.';
            }
        }
    }
}

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "error: missing puzzle input file" << std::endl;
        exit(EXIT_FAILURE);
    }

    std::ifstream input(argv[1]);
    std::string light_row;
    LightMatrix lights;
    while (std::getline(input, light_row)) {
        lights.push_back(light_row);
    }

    const int kNumSteps = 100;
    for (int i = 0; i < kNumSteps; ++i) {
        UpdateLights(lights);
    }

    int num_lights_on = 0;
    for (const std::string& row : lights) {
        for (const char& light : row) {
            if (light == '#') {
                num_lights_on++;
            }
        }
    }

    std::cout << "Answer: " << num_lights_on << std::endl;

    exit(EXIT_SUCCESS);
}
