#include <cstdint>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

enum class InstructionType {
    kCpy,
    kInc,
    kDec,
    kJnz,
    kTgl,
    kOut,
};

struct Instruction {
    InstructionType type;
    std::string arg1;
    std::string arg2;
};

class VirtualMachine {
   public:
    VirtualMachine(int a_value = 0);
    ~VirtualMachine() = default;
    VirtualMachine(const VirtualMachine&) = default;
    VirtualMachine(VirtualMachine&&) = default;

    VirtualMachine& operator=(const VirtualMachine&) = default;
    VirtualMachine& operator=(VirtualMachine&&) = default;

    int64_t ReadRegister(const std::string& reg_label) const {
        return registers_.at(reg_label);
    }

    bool Execute(std::vector<Instruction>& instructions,
                 const std::vector<int>& target_transmission);

    void Reset(int a_value);

   private:
    using RegisterMap = std::unordered_map<std::string, int64_t>;

    bool IsReg(const std::string& label) const {
        return (registers_.count(label) > 0);
    }

    int64_t EvaluateArg(const std::string& arg) const {
        return (IsReg(arg)) ? registers_.at(arg) : std::stoll(arg);
    }

    bool IsValidInstruction(const Instruction& inst, int num_inst) const;

    Instruction ToggleInstruction(const Instruction& inst) const;

    int ip_;
    RegisterMap registers_;
};

bool VirtualMachine::IsValidInstruction(const Instruction& inst,
                                        int num_inst) const {
    switch (inst.type) {
        case InstructionType::kInc:
        case InstructionType::kDec:
            if (!IsReg(inst.arg1)) {
                return false;
            }
            break;
        case InstructionType::kCpy:
            if (!IsReg(inst.arg2)) {
                return false;
            }
            break;
        case InstructionType::kJnz: {
            int64_t target_ip = ip_ + EvaluateArg(inst.arg2);
            if (target_ip < 0) {
                return false;
            }
            break;
        }
        case InstructionType::kTgl: {
            int64_t target_ip = ip_ + EvaluateArg(inst.arg1);
            if ((target_ip < 0) || (target_ip >= num_inst)) {
                return false;
            }
            break;
        }
        case InstructionType::kOut: {
            /* Assume all out instructions are valid in the input. */
            break;
        }
    }
    return true;
}

Instruction VirtualMachine::ToggleInstruction(const Instruction& inst) const {
    Instruction toggled_inst = inst;
    switch (inst.type) {
        case InstructionType::kInc:
            toggled_inst.type = InstructionType::kDec;
            break;
        case InstructionType::kDec:
        case InstructionType::kTgl:
        case InstructionType::kOut:
            toggled_inst.type = InstructionType::kInc;
            break;
        case InstructionType::kJnz:
            toggled_inst.type = InstructionType::kCpy;
            break;
        case InstructionType::kCpy:
            toggled_inst.type = InstructionType::kJnz;
            break;
    }
    return toggled_inst;
}

VirtualMachine::VirtualMachine(int a_value) : ip_(0) {
    registers_["a"] = a_value;
    registers_["b"] = 0;
    registers_["c"] = 0;
    registers_["d"] = 0;
}

bool VirtualMachine::Execute(std::vector<Instruction>& instructions,
                             const std::vector<int>& target_transmission) {
    std::vector<int> transmission;
    while ((ip_ >= 0) && (ip_ < instructions.size())) {
        const Instruction& inst = instructions[ip_];

        /* Skip over invalid instructions. */
        if (!IsValidInstruction(inst, instructions.size())) {
            ip_++;
            continue;
        }

        switch (inst.type) {
            case InstructionType::kCpy: {
                if (IsReg(inst.arg1)) {
                    registers_[inst.arg2] = registers_[inst.arg1];
                } else {
                    registers_[inst.arg2] = std::stoll(inst.arg1);
                }
                ip_++;
                break;
            }
            case InstructionType::kInc: {
                registers_[inst.arg1]++;
                ip_++;
                break;
            }
            case InstructionType::kDec: {
                registers_[inst.arg1]--;
                ip_++;
                break;
            }
            case InstructionType::kJnz: {
                if (0 != EvaluateArg(inst.arg1)) {
                    ip_ += EvaluateArg(inst.arg2);
                } else {
                    ip_++;
                }
                break;
            }
            case InstructionType::kTgl: {
                int64_t tgl_ip = ip_ + EvaluateArg(inst.arg1);
                instructions[tgl_ip] = ToggleInstruction(instructions[tgl_ip]);
                ip_++;
                break;
            }
            case InstructionType::kOut: {
                if (transmission.size() > target_transmission.size()) {
                    return false;
                } else if (transmission == target_transmission) {
                    return true;
                }
                transmission.push_back(EvaluateArg(inst.arg1));
                ip_++;
                break;
            }
        }
    }
    return false;
}

void VirtualMachine::Reset(int a_value) {
    ip_ = 0;
    registers_["a"] = a_value;
    registers_["b"] = 0;
    registers_["c"] = 0;
    registers_["d"] = 0;
}

Instruction ParseInstruction(const std::string& inst) {
    std::istringstream iss(inst);
    std::vector<std::string> tokens;
    std::copy(std::istream_iterator<std::string>(iss),
              std::istream_iterator<std::string>(), std::back_inserter(tokens));

    Instruction instruction;
    if (tokens[0] == "cpy") {
        instruction.type = InstructionType::kCpy;
        instruction.arg1 = tokens[1];
        instruction.arg2 = tokens[2];
    } else if (tokens[0] == "inc") {
        instruction.type = InstructionType::kInc;
        instruction.arg1 = tokens[1];
    } else if (tokens[0] == "dec") {
        instruction.type = InstructionType::kDec;
        instruction.arg1 = tokens[1];
    } else if (tokens[0] == "jnz") {
        instruction.type = InstructionType::kJnz;
        instruction.arg1 = tokens[1];
        instruction.arg2 = tokens[2];
    } else if (tokens[0] == "tgl") {
        instruction.type = InstructionType::kTgl;
        instruction.arg1 = tokens[1];
    } else if (tokens[0] == "out") {
        instruction.type = InstructionType::kOut;
        instruction.arg1 = tokens[1];
    }
    return instruction;
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

    std::vector<int> target_transmission(10, 0);
    for (int i = 1; i < target_transmission.size(); i += 2) {
        target_transmission[i] = 1;
    }

    int a_value = 0;
    VirtualMachine vm(a_value);
    while (!vm.Execute(instructions, target_transmission)) {
        a_value++;
        vm.Reset(a_value);
    }

    std::cout << "Answer: " << a_value << std::endl;

    exit(EXIT_SUCCESS);
}
