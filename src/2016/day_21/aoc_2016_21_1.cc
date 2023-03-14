#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>
#include <vector>

enum class InstructionType {
    kSwapPosition,
    kSwapLetter,
    kReverse,
    kRotateLeft,
    kRotateRight,
    kRotateOnPos,
    kMove,
};

struct Instruction {
    InstructionType type;
    std::string arg1;
    std::string arg2;
};

Instruction ParseInstruction(const std::string& text) {
    std::istringstream iss(text);
    std::vector<std::string> tokens;
    std::copy(std::istream_iterator<std::string>(iss),
              std::istream_iterator<std::string>(), std::back_inserter(tokens));

    Instruction inst;
    if ("swap" == tokens[0]) {
        if ("letter" == tokens[1]) {
            inst.type = InstructionType::kSwapLetter;
        } else {
            inst.type = InstructionType::kSwapPosition;
        }
        inst.arg1 = tokens[2];
        inst.arg2 = tokens[5];
    } else if ("reverse" == tokens[0]) {
        inst.type = InstructionType::kReverse;
        inst.arg1 = tokens[2];
        inst.arg2 = tokens[4];
    } else if ("move" == tokens[0]) {
        inst.type = InstructionType::kMove;
        inst.arg1 = tokens[2];
        inst.arg2 = tokens[5];
    } else if ("rotate" == tokens[0]) {
        if ("based" == tokens[1]) {
            inst.type = InstructionType::kRotateOnPos;
            inst.arg1 = tokens.back();
        } else {
            if ("left" == tokens[1]) {
                inst.type = InstructionType::kRotateLeft;
            } else {
                inst.type = InstructionType::kRotateRight;
            }
            inst.arg1 = tokens[2];
        }
    }
    return inst;
}

class Scrambler {
   public:
    Scrambler() = default;
    ~Scrambler() = default;

    Scrambler(const Scrambler&) = default;
    Scrambler(Scrambler&&) = default;
    Scrambler& operator=(const Scrambler&) = default;
    Scrambler& operator=(Scrambler&&) = default;

    std::string Scramble(const std::vector<Instruction>& instructions,
                         std::string seed);

   private:
    void SwapPosition(std::string& s, int p1, int p2) const;
    void SwapLetter(std::string& s, char l1, char l2) const;
    void ReverseRange(std::string& s, int start, int end) const;
    void RotateLeft(std::string& s, int r) const;
    void RotateRight(std::string& s, int r) const;
    void MoveFromTo(std::string& s, int from, int to) const;
    void RotateBasedOnLetterPos(std::string& s, char letter) const;
};

void Scrambler::SwapPosition(std::string& s, int p1, int p2) const {
    std::swap(s[p1], s[p2]);
}

void Scrambler::SwapLetter(std::string& s, char l1, char l2) const {
    int l1_index = s.find(l1);
    int l2_index = s.find(l2);
    std::swap(s[l1_index], s[l2_index]);
}

void Scrambler::ReverseRange(std::string& s, int start, int end) const {
    std::reverse(s.begin() + start, s.begin() + end + 1);
}

void Scrambler::RotateLeft(std::string& s, int r) const {
    r = r % s.size();
    std::rotate(s.begin(), s.begin() + r, s.end());
}

void Scrambler::RotateRight(std::string& s, int r) const {
    r = r % s.size();
    std::rotate(s.rbegin(), s.rbegin() + r, s.rend());
}

void Scrambler::MoveFromTo(std::string& s, int from, int to) const {
    char src_char = s[from];
    s.erase(s.begin() + from);
    s.insert(s.begin() + to, src_char);
}

void Scrambler::RotateBasedOnLetterPos(std::string& s, char letter) const {
    int index = s.find(letter);
    RotateRight(s, index + 1);
    if (index >= 4) {
        RotateRight(s, 1);
    }
}

std::string Scrambler::Scramble(const std::vector<Instruction>& instructions,
                                std::string seed) {
    for (const Instruction& inst : instructions) {
        switch (inst.type) {
            case InstructionType::kSwapPosition:
                SwapPosition(seed, std::stoi(inst.arg1), std::stoi(inst.arg2));
                break;
            case InstructionType::kSwapLetter:
                SwapLetter(seed, inst.arg1[0], inst.arg2[0]);
                break;
            case InstructionType::kReverse:
                ReverseRange(seed, std::stoi(inst.arg1), std::stoi(inst.arg2));
                break;
            case InstructionType::kRotateLeft:
                RotateLeft(seed, std::stoi(inst.arg1));
                break;
            case InstructionType::kRotateRight:
                RotateRight(seed, std::stoi(inst.arg1));
                break;
            case InstructionType::kRotateOnPos:
                RotateBasedOnLetterPos(seed, inst.arg1[0]);
                break;
            case InstructionType::kMove:
                MoveFromTo(seed, std::stoi(inst.arg1), std::stoi(inst.arg2));
                break;
        }
    }
    return seed;
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

    Scrambler pass_scrambler;
    const std::string kPassword("abcdefgh");
    std::cout << "Answer: " << pass_scrambler.Scramble(instructions, kPassword)
              << std::endl;

    exit(EXIT_SUCCESS);
}
