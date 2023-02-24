#include <cstdlib>
#include <fstream>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using RuleMap = std::unordered_map<std::string, std::vector<std::string>>;

void ParseRule(const std::string& rule, RuleMap& rules) {
    std::istringstream iss(rule);
    std::vector<std::string> tokens;
    std::copy(std::istream_iterator<std::string>(iss),
              std::istream_iterator<std::string>(), std::back_inserter(tokens));

    rules[tokens[0]].push_back(tokens[2]);
}

std::vector<int> GetInsertLocations(const std::string& replacement,
                                    const std::string& molecule) {
    std::vector<int> insert_locations;
    int window_size = replacement.size();
    for (int i = 0; (molecule.size() - i) >= window_size; ++i) {
        if (replacement == molecule.substr(i, window_size)) {
            insert_locations.push_back(i);
        }
    }
    return insert_locations;
}

int GetUniqueMoleculeCount(const RuleMap& rules, const std::string& molecule) {
    std::unordered_set<std::string> unique_molecules;
    for (const auto& [src, replacements] : rules) {
        std::vector<int> insert_locations = GetInsertLocations(src, molecule);
        for (const std::string& replacement : replacements) {
            for (const int& i : insert_locations) {
                std::string new_molecule = molecule.substr(0, i) + replacement +
                                           molecule.substr(i + src.size());
                unique_molecules.insert(new_molecule);
            }
        }
    }
    return unique_molecules.size();
}

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "error: missing puzzle input file" << std::endl;
        exit(EXIT_FAILURE);
    }

    std::ifstream input(argv[1]);
    std::string rule;
    RuleMap rules;
    while (std::getline(input, rule) && !rule.empty()) {
        ParseRule(rule, rules);
    }

    std::string molecule;
    std::getline(input, molecule);

    std::cout << "Answer: " << GetUniqueMoleculeCount(rules, molecule)
              << std::endl;

    exit(EXIT_SUCCESS);
}
