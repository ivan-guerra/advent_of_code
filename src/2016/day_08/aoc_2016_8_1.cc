#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>
#include <vector>

using Screen = std::vector<std::vector<int>>;

enum Operation {
    kRect,
    kRotateRow,
    kRotateCol,
};

struct Instruction {
    Operation op;
    int arg1 = 0;
    int arg2 = 0;
};

void RotateRight(std::vector<int>& values, int shift) {
    int rotation = shift % values.size();
    std::rotate(values.rbegin(), values.rbegin() + rotation, values.rend());
}

Instruction ParseInstruction(const std::string& text) {
    std::istringstream iss(text);
    std::vector<std::string> tokens;
    std::copy(std::istream_iterator<std::string>(iss),
              std::istream_iterator<std::string>(), std::back_inserter(tokens));

    Instruction inst;
    if (tokens[0] == "rect") {
        int brk = tokens[1].find('x');
        inst.op = Operation::kRect;
        inst.arg1 = std::stoi(tokens[1].substr(0, brk));
        inst.arg2 = std::stoi(tokens[1].substr(brk + 1));
    } else if (tokens[0] == "rotate") {
        if (tokens[1] == "column") {
            inst.op = Operation::kRotateCol;
        } else if (tokens[1] == "row") {
            inst.op = Operation::kRotateRow;
        }
        int brk = tokens[2].find('=');
        inst.arg1 = std::stoi(tokens[2].substr(brk + 1));
        inst.arg2 = std::stoi(tokens[4]);
    }
    return inst;
}

int CheckVoltage(const Screen& screen) {
    int voltage = 0;
    for (const auto& row : screen) {
        for (const int& val : row) {
            if (val) {
                voltage++;
            }
        }
    }
    return voltage;
}

void ExecuteRectInstruction(Screen& screen, int width, int height) {
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            screen[i][j] = 1;
        }
    }
}

void ExecuteRowRotation(Screen& screen, int row, int shift) {
    RotateRight(screen[row], shift);
}

void ExecuteColRotation(Screen& screen, int col, int shift) {
    std::vector<int> col_cp(screen.size(), 0);
    for (int i = 0; i < screen.size(); ++i) {
        col_cp[i] = screen[i][col];
    }

    RotateRight(col_cp, shift);

    for (int i = 0; i < screen.size(); ++i) {
        screen[i][col] = col_cp[i];
    }
}

void ExecuteInstructions(Screen& screen,
                         const std::vector<Instruction>& instructions) {
    for (const Instruction& inst : instructions) {
        switch (inst.op) {
            case Operation::kRect:
                ExecuteRectInstruction(screen, inst.arg1, inst.arg2);
                break;
            case Operation::kRotateRow:
                ExecuteRowRotation(screen, inst.arg1, inst.arg2);
                break;
            case Operation::kRotateCol:
                ExecuteColRotation(screen, inst.arg1, inst.arg2);
                break;
        }
    }
}

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "error: missing puzzle input file" << std::endl;
        exit(EXIT_FAILURE);
    }

    std::ifstream input(argv[1]);
    std::string instruction;
    std::vector<Instruction> instructions;
    while (std::getline(input, instruction)) {
        instructions.push_back(ParseInstruction(instruction));
    }

    static const int kWidth = 50;
    static const int kHeight = 6;
    Screen screen(kHeight, std::vector<int>(kWidth, 0));
    ExecuteInstructions(screen, instructions);

    std::cout << "Answer: " << CheckVoltage(screen) << std::endl;

    exit(EXIT_SUCCESS);
}
