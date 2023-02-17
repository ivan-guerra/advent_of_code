#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

int GetRibbonLength(const std::string& dimensions) {
    std::istringstream iss(dimensions);
    std::string token;
    std::vector<int> dims;
    while (std::getline(iss, token, 'x')) {
        dims.push_back(std::stoi(token));
    }

    int length = dims[0];
    int width = dims[1];
    int height = dims[2];
    int min_perimeter =
        std::min(2 * (width + length),
                 std::min(2 * (width + height), 2 * (length + height)));

    return (min_perimeter + (length * width * height));
}

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "error: missing puzzle input file" << std::endl;
        exit(EXIT_FAILURE);
    }

    std::ifstream input(argv[1]);
    std::string gift;

    int total_ribbon_length = 0;
    while (std::getline(input, gift)) {
        total_ribbon_length += GetRibbonLength(gift);
    }
    std::cout << "Answer: " << total_ribbon_length << std::endl;

    exit(EXIT_SUCCESS);
}
