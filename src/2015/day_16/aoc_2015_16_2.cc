#include <cstdlib>
#include <fstream>
#include <iostream>
#include <iterator>
#include <list>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

using AuntAttrs = std::unordered_map<std::string, int>;

struct Aunt {
    int id = 0;
    std::unordered_map<std::string, int> attrs;
};

Aunt ParseAunt(const std::string& sue) {
    std::istringstream iss(sue);
    std::vector<std::string> tokens;
    std::copy(std::istream_iterator<std::string>(iss),
              std::istream_iterator<std::string>(), std::back_inserter(tokens));

    auto strip_last_char = [](const std::string& s) {
        return (s.substr(0, s.size() - 1));
    };

    Aunt ret;
    ret.id = std::stoi(strip_last_char(tokens[1]));
    ret.attrs[strip_last_char(tokens[2])] =
        std::stoi(strip_last_char(tokens[3]));
    ret.attrs[strip_last_char(tokens[4])] =
        std::stoi(strip_last_char(tokens[5]));
    ret.attrs[strip_last_char(tokens[6])] = std::stoi(tokens[7]);
    return ret;
}

void FilterAunts(std::list<Aunt>& aunts, const AuntAttrs& target_sue_attrs) {
    bool has_mismatch = false;
    for (auto it = aunts.begin(); it != aunts.end();) {
        has_mismatch = false;
        for (const auto& [attr, value] : it->attrs) {
            if ((attr == "cats") || (attr == "trees")) {
                if (value <= target_sue_attrs.at(attr)) {
                    has_mismatch = true;
                }
            } else if ((attr == "pomeranians") || (attr == "goldfish")) {
                if (value >= target_sue_attrs.at(attr)) {
                    has_mismatch = true;
                }
            } else if (target_sue_attrs.at(attr) != value) {
                has_mismatch = true;
            }

            if (has_mismatch) {
                it = aunts.erase(it);
                break;
            }
        }
        if (!has_mismatch) {
            it++;
        }
    }
}

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "error: missing puzzle input file" << std::endl;
        exit(EXIT_FAILURE);
    }

    AuntAttrs target_sue_attrs = {
        {"children", 3}, {"cats", 7},    {"samoyeds", 2}, {"pomeranians", 3},
        {"akitas", 0},   {"vizslas", 0}, {"goldfish", 5}, {"trees", 3},
        {"cars", 2},     {"perfumes", 1}};
    std::ifstream input(argv[1]);
    std::string aunt;
    std::list<Aunt> aunts;
    while (std::getline(input, aunt)) {
        aunts.insert(aunts.begin(), ParseAunt(aunt));
    }

    FilterAunts(aunts, target_sue_attrs);

    std::cout << "Answer: " << aunts.begin()->id << std::endl;

    exit(EXIT_SUCCESS);
}
