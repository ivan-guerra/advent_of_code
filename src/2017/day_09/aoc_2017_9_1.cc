#include <cstdlib>
#include <fstream>
#include <iostream>
#include <stack>
#include <string>

int ComputeGroupScore(const std::string& text) {
    int score = 0;
    std::stack<char> opening_brackets;

    int i = 0;
    opening_brackets.push(text[i++]);
    while (!opening_brackets.empty()) {
        if (text[i] == '{') {
            opening_brackets.push('{');
        } else if (text[i] == '}') {
            score += opening_brackets.size();
            opening_brackets.pop();
        } else if (text[i] == '<') {
            while (text[i] != '>') {
                i += (text[i] == '!') ? 2 : 1;
            }
        }
        i++;
    }
    return score;
}

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "error: missing puzzle input file" << std::endl;
        exit(EXIT_FAILURE);
    }

    std::ifstream input(argv[1]);
    std::string text;
    while (std::getline(input, text)) {
        std::cout << "Answer: " << ComputeGroupScore(text) << std::endl;
    }

    exit(EXIT_SUCCESS);
}
