#include <cstdlib>
#include <fstream>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>
#include <vector>

using SideLengthMatrix = std::vector<std::vector<int>>;

struct Triangle {
    int a = 0;
    int b = 0;
    int c = 0;
};

bool IsTriangle(const Triangle& triangle) {
    int ab_sum = triangle.a + triangle.b;
    int ac_sum = triangle.a + triangle.c;
    int bc_sum = triangle.b + triangle.c;

    return ((ab_sum > triangle.c) && (ac_sum > triangle.b) &&
            (bc_sum > triangle.a));
}

std::vector<Triangle> BuildTriangles(const SideLengthMatrix& slm) {
    std::vector<Triangle> triangles;
    for (int i = 0; i < slm.size(); i += 3) {
        for (int j = 0; j < 3; ++j) {
            Triangle candidate = {
                .a = slm[i][j],
                .b = slm[i + 1][j],
                .c = slm[i + 2][j],
            };
            if (IsTriangle(candidate)) {
                triangles.push_back(candidate);
            }
        }
    }
    return triangles;
}

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "error: missing puzzle input file" << std::endl;
        exit(EXIT_FAILURE);
    }

    std::ifstream input(argv[1]);
    std::string side_lengths;
    SideLengthMatrix side_len_mat;
    while (std::getline(input, side_lengths)) {
        std::istringstream iss(side_lengths);
        std::vector<std::string> tokens;
        std::copy(std::istream_iterator<std::string>(iss),
                  std::istream_iterator<std::string>(),
                  std::back_inserter(tokens));
        std::vector<int> side_lengths_int;
        for (const std::string& token : tokens) {
            side_lengths_int.push_back(std::stoi(token));
        }
        side_len_mat.push_back(side_lengths_int);
    }

    std::vector<Triangle> triangles = BuildTriangles(side_len_mat);
    std::cout << "Answer: " << triangles.size() << std::endl;

    exit(EXIT_SUCCESS);
}
