#include <cstdint>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>
#include <vector>

int ParseGeneratorSeed(const std::string& text) {
    std::istringstream iss(text);
    std::vector<std::string> tokens;
    std::copy(std::istream_iterator<std::string>(iss),
              std::istream_iterator<std::string>(), std::back_inserter(tokens));

    return std::stoi(tokens.back());
}

int CountMatches(int gen_a_seed, int gen_b_seed, int threshold) {
    static const uint64_t kGenAFactor = 16807;
    static const uint64_t kGenBFactor = 48271;
    static const uint64_t kMod = 2147483647;

    uint64_t curr_a = gen_a_seed;
    uint64_t curr_b = gen_b_seed;
    uint64_t mask = 0xFFFF;
    int matches = 0;
    for (int i = 0; i < threshold; ++i) {
        curr_a = (curr_a * kGenAFactor) % kMod;
        curr_b = (curr_b * kGenBFactor) % kMod;
        if ((curr_a & mask) == (curr_b & mask)) {
            matches++;
        }
    }
    return matches;
}

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "error: missing puzzle input file" << std::endl;
        exit(EXIT_FAILURE);
    }

    std::ifstream input(argv[1]);
    std::string text;
    std::getline(input, text);
    int gen_a_seed = ParseGeneratorSeed(text);
    std::getline(input, text);
    int gen_b_seed = ParseGeneratorSeed(text);

    const int kThreshold = 40 * 1000000;
    std::cout << "Answer: " << CountMatches(gen_a_seed, gen_b_seed, kThreshold)
              << std::endl;

    exit(EXIT_SUCCESS);
}
