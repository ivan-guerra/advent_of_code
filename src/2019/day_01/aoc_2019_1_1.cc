#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>

int CalculateFuelFromMass(int mass) {
    static const int kDivisor = 3;
    static const int kSubtrahend = 2;
    return (mass / kDivisor) - kSubtrahend;
}

int main(int argc, char **argv) {
    if (argc != 2) {
        std::cerr << "error: missing puzzle input file" << std::endl;
        exit(EXIT_FAILURE);
    }

    std::ifstream input(argv[1]);
    std::string line;
    int fuel_sum = 0;
    while (std::getline(input, line)) {
        fuel_sum += CalculateFuelFromMass(std::stoi(line));
    }

    std::cout << "Answer: " << fuel_sum << std::endl;

    exit(EXIT_SUCCESS);
}
