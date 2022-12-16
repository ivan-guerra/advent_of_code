#include <algorithm>
#include <climits>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <numeric>
#include <queue>
#include <string>
#include <vector>

int FindKLargestCaloriesSum(const std::string& input, int k) {
    std::ifstream calorie_if(input);
    if (!calorie_if.is_open()) {
        std::cerr << "invalid input file: " << input << std::endl;
        return -1;
    }

    std::string line;
    int curr_calories = 0;
    std::priority_queue<int> calories;
    while (std::getline(calorie_if, line)) {
        if (line.empty()) {
            calories.push(curr_calories);
            curr_calories = 0;
        } else {
            curr_calories += std::stoi(line);
        }
    }

    int sum = 0;
    while (k--) {
        sum += calories.top();
        calories.pop();
    }

    return sum;
}

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "usage: aoc_2 ELF_CALORIES" << std::endl;
        exit(EXIT_FAILURE);
    }

    std::cout << "Top 3 Calories Sum To: "
              << FindKLargestCaloriesSum(argv[1], 3) << std::endl;

    exit(EXIT_SUCCESS);
}
