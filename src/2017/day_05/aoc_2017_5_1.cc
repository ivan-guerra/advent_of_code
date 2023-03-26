#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

int MinNumOfJumps(std::vector<int>& offsets) {
    int ip = 0;
    int num_jumps = 0;
    while ((ip >= 0) && (ip < offsets.size())) {
        int old_ip = ip;
        ip += offsets[ip];

        if (offsets[old_ip] >= 3) {
            offsets[old_ip]--;
        } else {
            offsets[old_ip]++;
        }
        num_jumps++;
    }
    return num_jumps;
}

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "error: missing puzzle input file" << std::endl;
        exit(EXIT_FAILURE);
    }

    std::ifstream input(argv[1]);
    std::string offset;
    std::vector<int> offsets;
    while (std::getline(input, offset)) {
        offsets.push_back(std::stoi(offset));
    }

    std::cout << "Answer: " << MinNumOfJumps(offsets) << std::endl;

    exit(EXIT_SUCCESS);
}
