#include <algorithm>
#include <climits>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>

std::vector<std::vector<int>> NChooseK(const std::vector<int>& values, int k) {
    std::vector<int> aux(values.size(), 1);
    std::fill(aux.begin(), aux.begin() + values.size() - k, 0);

    std::vector<std::vector<int>> combinations;
    do {
        std::vector<int> combo;
        for (int i = 0; i < aux.size(); ++i) {
            if (aux[i]) {
                combo.push_back(values[i]);
            }
        }
        combinations.push_back(combo);
    } while (std::next_permutation(aux.begin(), aux.end()));
    return combinations;
}

unsigned long long FindMinQe(const std::vector<int>& weights, int target) {
    unsigned long long min_qe = ULLONG_MAX;
    for (int i = 1; i < weights.size(); ++i) {
        std::vector<std::vector<int>> candidates = NChooseK(weights, i);
        for (const std::vector<int>& c : candidates) {
            int sum = 0;
            unsigned long long product = 1;
            for (const int& val : c) {
                sum += val;
                product *= val;
            }

            if (sum == target) {
                min_qe = std::min(min_qe, product);
            }
        }
        if (min_qe != ULLONG_MAX) {
            return min_qe;
        }
    }
    return -1;
}

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "error: missing puzzle input file" << std::endl;
        exit(EXIT_FAILURE);
    }

    std::ifstream input(argv[1]);
    std::string weight;
    std::vector<int> weights;
    while (std::getline(input, weight)) {
        weights.push_back(std::stoi(weight));
    }

    int partitions = 4;
    int target =
        std::accumulate(weights.begin(), weights.end(), 0) / partitions;
    std::cout << "Answer: " << FindMinQe(weights, target) << std::endl;

    exit(EXIT_SUCCESS);
}
