#include <cstdlib>
#include <fstream>
#include <iostream>
#include <regex>
#include <stack>
#include <vector>

struct CargoAction {
    int quantity;
    int from;
    int to;

    CargoAction(int quant, int src, int dst)
        : quantity(quant), from(src), to(dst) {}
};

using ActionList = std::vector<CargoAction>;
using BoxStacks = std::vector<std::stack<char>>;

ActionList ParseCargoActions(std::ifstream& input) {
    ActionList actions;
    std::regex re("move ([0-9]+) from ([0-9]+) to ([0-9]+)");
    std::string line;
    while (std::getline(input, line)) {
        std::smatch matches;
        if (std::regex_search(line, matches, re) && matches.size() == 4) {
            actions.emplace_back(std::stoi(matches[1]), std::stoi(matches[2]),
                                 std::stoi(matches[3]));
        }
    }
    return actions;
}

BoxStacks ParseBoxStacks(std::ifstream& input) {
    // Read in stack input lines including the row listing stack numbers.
    std::vector<std::string> lines;
    std::string line;
    while (std::getline(input, line) && !line.empty()) lines.push_back(line);

    BoxStacks stacks;
    std::vector<std::size_t> item_cols;

    // Initialize the list of stacks and save off the col index at which each
    // stack item first appears.
    int i = 0;
    const std::string& stack_nums = lines.back();
    while (i < static_cast<int>(stack_nums.size())) {
        if (!std::isspace(stack_nums[i])) {
            item_cols.push_back(i);
            stacks.emplace_back();
            while (!std::isspace(stack_nums[i])) i++;
        }
        i++;
    }

    // Walk the stack lines in reverse.
    for (i = lines.size() - 2; i >= 0; --i) {
        // Populate each stack using item_cols are an indicator of where
        // the current stacks item is in the input line.
        for (std::size_t j = 0; j < item_cols.size(); ++j) {
            if (item_cols[j] >= lines[i].size()) continue;

            char item = lines[i][item_cols[j]];
            if (!std::isspace(item)) stacks[j].push(item);
        }
    }
    return stacks;
}

void PerformCargoAction(const CargoAction& action, BoxStacks& stacks) {
    auto& src = stacks[action.from - 1];
    std::stack<char> aux;
    for (int i = 0; i < action.quantity; ++i) {
        aux.push(src.top());
        src.pop();
    }

    auto& dst = stacks[action.to - 1];
    while (!aux.empty()) {
        dst.push(aux.top());
        aux.pop();
    }
}

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "usage: aoc INPUT_FILE" << std::endl;
        exit(EXIT_FAILURE);
    }

    std::ifstream input(argv[1]);
    BoxStacks stacks = ParseBoxStacks(input);
    ActionList actions = ParseCargoActions(input);

    for (const CargoAction& action : actions)
        PerformCargoAction(action, stacks);

    for (const std::stack<char>& s : stacks) std::cout << s.top();
    std::cout << std::endl;

    exit(EXIT_SUCCESS);
}
