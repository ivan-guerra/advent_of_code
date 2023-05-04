#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

std::vector<int> GetDigits(int n) {
    if (!n) {
        return {0};
    }

    std::vector<int> digits;
    while (n) {
        digits.push_back(n % 10);
        n /= 10;
    }
    std::reverse(digits.begin(), digits.end());

    return digits;
}

std::vector<int> RunSimulation(int num_recipes) {
    const int kNumExtraRecipes = 10;
    const int kTargetRecipes = num_recipes + kNumExtraRecipes;
    std::vector<int> recipes = {3, 7};

    int elf1 = 0;
    int elf2 = 1;
    while (recipes.size() < kTargetRecipes) {
        int sum = recipes[elf1] + recipes[elf2];
        std::vector<int> sum_digits = GetDigits(sum);
        recipes.insert(recipes.end(), sum_digits.begin(), sum_digits.end());

        elf1 = (elf1 + 1 + recipes[elf1]) % recipes.size();
        elf2 = (elf2 + 1 + recipes[elf2]) % recipes.size();
    }
    return std::vector<int>(recipes.begin() + num_recipes,
                            recipes.begin() + kTargetRecipes);
}

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "error: missing puzzle input file" << std::endl;
        exit(EXIT_FAILURE);
    }

    std::ifstream input(argv[1]);
    std::string line;
    std::getline(input, line);
    int num_recipes = std::stoi(line);
    std::vector<int> recipes = RunSimulation(num_recipes);

    std::cout << "Answer: ";
    for (const int& recipe : recipes) {
        std::cout << recipe;
    }
    std::cout << std::endl;

    exit(EXIT_SUCCESS);
}
