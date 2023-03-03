#include <cstdlib>
#include <fstream>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>
#include <vector>

struct Triangle {
    int a = 0;
    int b = 0;
    int c = 0;
};

Triangle ParseTriangle(const std::string& side_lengths) {
    std::istringstream iss(side_lengths);
    std::vector<std::string> tokens;
    std::copy(std::istream_iterator<std::string>(iss),
              std::istream_iterator<std::string>(), std::back_inserter(tokens));

    Triangle triangle = {
        .a = std::stoi(tokens[0]),
        .b = std::stoi(tokens[1]),
        .c = std::stoi(tokens[2]),
    };
    return triangle;
}

bool IsTriangle(const Triangle& triangle) {
    int ab_sum = triangle.a + triangle.b;
    int ac_sum = triangle.a + triangle.c;
    int bc_sum = triangle.b + triangle.c;

    return ((ab_sum > triangle.c) && (ac_sum > triangle.b) &&
            (bc_sum > triangle.a));
}

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "error: missing puzzle input file" << std::endl;
        exit(EXIT_FAILURE);
    }

    std::ifstream input(argv[1]);
    std::string side_length;
    std::vector<Triangle> triangles;
    while (std::getline(input, side_length)) {
        triangles.push_back(ParseTriangle(side_length));
    }

    int num_triangles = 0;
    for (const Triangle& t : triangles) {
        num_triangles += (IsTriangle(t)) ? 1 : 0;
    }

    std::cout << "Answer: " << num_triangles << std::endl;

    exit(EXIT_SUCCESS);
}
