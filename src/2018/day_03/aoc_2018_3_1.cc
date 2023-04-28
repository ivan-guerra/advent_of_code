#include <cstdlib>
#include <fstream>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>
#include <vector>

using Position = std::pair<int, int>;
using Dim = std::pair<int, int>;
using Fabric = std::vector<std::vector<int>>;

struct Claim {
    int id;
    Position pos;
    Dim dim;
};

Claim ParseClaim(const std::string& text) {
    std::istringstream iss(text);
    std::vector<std::string> words;
    std::copy(std::istream_iterator<std::string>(iss),
              std::istream_iterator<std::string>(), std::back_inserter(words));

    Claim claim;
    claim.id = std::stoi(words[0].substr(1));

    int comma_idx = words[2].find(',');
    int colon_idx = words[2].find(':');
    claim.pos.first = std::stoi(words[2].substr(0, comma_idx));
    claim.pos.second =
        std::stoi(words[2].substr(comma_idx + 1, colon_idx - comma_idx - 1));

    int x_idx = words[3].find('x');
    claim.dim.first = std::stoi(words[3].substr(0, x_idx));
    claim.dim.second = std::stoi(words[3].substr(x_idx + 1));

    return claim;
}

void ApplyClaim(Fabric& fabric, const Claim& claim) {
    for (int i = claim.pos.second; i < (claim.pos.second + claim.dim.second);
         ++i) {
        for (int j = claim.pos.first; j < (claim.pos.first + claim.dim.first);
             ++j) {
            fabric[i][j]++;
        }
    }
}

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "error: missing puzzle input file" << std::endl;
        exit(EXIT_FAILURE);
    }

    std::ifstream input(argv[1]);
    std::string line;
    std::vector<Claim> claims;
    while (std::getline(input, line)) {
        claims.push_back(ParseClaim(line));
    }

    const int kFabricDim = 1000;
    Fabric fabric(kFabricDim, std::vector<int>(kFabricDim, 0));
    for (const Claim& claim : claims) {
        ApplyClaim(fabric, claim);
    }

    int overlaps = 0;
    for (int i = 0; i < kFabricDim; ++i) {
        for (int j = 0; j < kFabricDim; ++j) {
            if (fabric[i][j] >= 2) {
                overlaps++;
            }
        }
    }
    std::cout << "Answer: " << overlaps << std::endl;

    exit(EXIT_SUCCESS);
}
