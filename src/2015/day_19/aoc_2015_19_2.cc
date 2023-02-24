#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>

/* This solutions was taken from:
 * https://www.reddit.com/r/adventofcode/comments/3xflz8/day_19_solutions */

int CountSubstr(const std::string& substr, const std::string& search) {
    int window_size = substr.size();
    int count = 0;
    for (int i = 0; (search.size() - i) >= window_size; ++i) {
        if (search.substr(i, window_size) == substr) {
            count++;
        }
    }
    return count;
}

int GetMinStepsToFabricate(const std::string& molecule) {
    int symbols = 0;
    for (const char& c : molecule) {
        if (std::isupper(c)) {
            symbols++;
        }
    }
    return (symbols - CountSubstr("Rn", molecule) -
            CountSubstr("Ar", molecule) - (2 * CountSubstr("Y", molecule)) - 1);
}

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "error: missing puzzle input file" << std::endl;
        exit(EXIT_FAILURE);
    }

    std::ifstream input(argv[1]);
    std::string rule;
    while (std::getline(input, rule) && !rule.empty()) {
    }

    std::string molecule;
    std::getline(input, molecule);

    std::cout << "Answer: " << GetMinStepsToFabricate(molecule) << std::endl;

    exit(EXIT_SUCCESS);
}
