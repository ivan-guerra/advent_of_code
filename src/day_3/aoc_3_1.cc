#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

char GetCommonItem(const std::string& rucksack) {
    std::unordered_set<char> compartment_one;
    int n = rucksack.size();
    for (int i = 0; i < (n / 2); ++i) compartment_one.insert(rucksack[i]);

    for (std::size_t i = (n / 2); i < rucksack.size(); ++i) {
        if (compartment_one.count(rucksack[i])) return rucksack[i];
    }
    return -1;
}

int SumOfCommonItemPriorities(const std::vector<std::string>& rucksacks,
                              const std::unordered_map<char, int>& priorities) {
    int sum = 0;
    for (const std::string& rucksack : rucksacks) {
        sum += priorities.at(GetCommonItem(rucksack));
    }
    return sum;
}

std::vector<std::string> LoadRucksacks(const std::string& input) {
    std::ifstream rucksack_if(input);

    std::string line;
    std::vector<std::string> rucksacks;
    while (std::getline(rucksack_if, line)) {
        rucksacks.push_back(line);
    }
    return rucksacks;
}

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "usage: aoc INPUT_FILE" << std::endl;
        exit(EXIT_FAILURE);
    }

    std::unordered_map<char, int> priorities;
    for (int i = 'a'; i <= 'z'; ++i)
        priorities[static_cast<char>(i)] = i - 'a' + 1;
    for (int i = 'A'; i <= 'Z'; ++i)
        priorities[static_cast<char>(i)] = i - 'A' + 27;

    std::vector<std::string> rucksacks = LoadRucksacks(argv[1]);
    std::cout << "Sum of Common Item Priorities = "
              << SumOfCommonItemPriorities(rucksacks, priorities) << std::endl;

    exit(EXIT_SUCCESS);
}
