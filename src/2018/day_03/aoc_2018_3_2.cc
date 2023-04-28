#include <cstdlib>
#include <fstream>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>
#include <unordered_set>
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

void ApplyClaim(Fabric& fabric, std::unordered_set<int>& overlapping_ids,
                const Claim& claim) {
    for (int i = claim.pos.second; i < (claim.pos.second + claim.dim.second);
         ++i) {
        for (int j = claim.pos.first; j < (claim.pos.first + claim.dim.first);
             ++j) {
            if (fabric[i][j]) {
                overlapping_ids.insert(fabric[i][j]);
                overlapping_ids.insert(claim.id);
            }
            fabric[i][j] = claim.id;
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
    std::unordered_set<int> overlapping_ids;
    for (const Claim& claim : claims) {
        ApplyClaim(fabric, overlapping_ids, claim);
    }

    for (const Claim& claim : claims) {
        if (!overlapping_ids.count(claim.id)) {
            std::cout << "Answer: " << claim.id << std::endl;
            break;
        }
    }

    exit(EXIT_SUCCESS);
}
