#include <cstdlib>
#include <fstream>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

struct Instruction {
    std::string mnemonic;
    std::vector<std::string> operands;
};

class VirtualMachine {
   public:
    VirtualMachine();
    ~VirtualMachine() = default;
    VirtualMachine(const VirtualMachine&) = delete;
    VirtualMachine& operator=(const VirtualMachine&) = delete;
    VirtualMachine(VirtualMachine&&) = delete;
    VirtualMachine& operator=(VirtualMachine&&) = delete;

    int GetRegisterA() const { return registers_.at("a"); }
    int GetRegisterB() const { return registers_.at("b"); }

    void Execute(const std::vector<Instruction>& source);

   private:
    bool IsValidInstruction(const std::vector<Instruction>& source) const {
        return ((ip_ >= 0) && (ip_ < source.size()));
    }

    int ip_;
    std::unordered_map<std::string, unsigned int> registers_;
};

VirtualMachine::VirtualMachine() : ip_(0) {
    registers_["a"] = 0;
    registers_["b"] = 0;
}

void VirtualMachine::Execute(const std::vector<Instruction>& source) {
    while (IsValidInstruction(source)) {
        const Instruction& inst = source[ip_];
        if (inst.mnemonic == "hlf") {
            registers_[inst.operands[0]] /= 2;
            ip_++;
        } else if (inst.mnemonic == "tpl") {
            registers_[inst.operands[0]] *= 3;
            ip_++;
        } else if (inst.mnemonic == "inc") {
            registers_[inst.operands[0]]++;
            ip_++;
        } else if (inst.mnemonic == "jmp") {
            ip_ += std::stoi(inst.operands[0]);
        } else if (inst.mnemonic == "jie") {
            if (0 == (registers_[inst.operands[0]] % 2)) {
                ip_ += std::stoi(inst.operands[1]);
            } else {
                ip_++;
            }
        } else if (inst.mnemonic == "jio") {
            if (1 == (registers_[inst.operands[0]])) {
                ip_ += std::stoi(inst.operands[1]);
            } else {
                ip_++;
            }
        }
    }
}

Instruction ParseInstruction(const std::string& source) {
    std::istringstream iss(source);
    std::vector<std::string> tokens;
    std::copy(std::istream_iterator<std::string>(iss),
              std::istream_iterator<std::string>(), std::back_inserter(tokens));

    /* Strip the trailing ',' on instructions with two operands. */
    if (tokens.size() == 3) {
        tokens[1] = tokens[1].substr(0, tokens[1].size() - 1);
    }

    Instruction inst = {
        .mnemonic = tokens[0],
        .operands = std::vector<std::string>(tokens.begin() + 1, tokens.end()),
    };
    return inst;
}

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "error: missing puzzle input file" << std::endl;
        exit(EXIT_FAILURE);
    }

    std::ifstream input(argv[1]);
    std::vector<Instruction> source_code;
    std::string line;
    while (std::getline(input, line)) {
        source_code.push_back(ParseInstruction(line));
    }

    VirtualMachine vm;
    vm.Execute(source_code);

    std::cout << "Answer: " << vm.GetRegisterB() << std::endl;

    exit(EXIT_SUCCESS);
}
