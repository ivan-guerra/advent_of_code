#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <iterator>
#include <memory>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

struct Instruction;
struct InputInstruction;
struct OutputInstruction;

using InstructionPtr = std::shared_ptr<Instruction>;
using InputInstructionPtr = std::shared_ptr<InputInstruction>;
using OutputInstructionPtr = std::shared_ptr<OutputInstruction>;

enum InstructionType {
    kInput,
    kOutput,
};

struct Instruction {
    InstructionType type;
    virtual ~Instruction() = default;
};

struct InputInstruction : public Instruction {
    int dst_bot = -1;
    int value = -1;
};

struct OutputInstruction : public Instruction {
    int src_bot = -1;
    std::string low_target;
    int low_id = -1;
    std::string high_target;
    int high_id = -1;
};

struct Bot {
    std::vector<int> chips;

    void InsertChip(int val) {
        if (std::find(std::begin(chips), std::end(chips), val) !=
            std::end(chips)) {
            return;
        }
        chips.push_back(val);
    }
    int GetLow() const {
        return (chips.size() == 2) ? std::min(chips[0], chips[1]) : -1;
    }
    int GetHigh() const {
        return (chips.size() == 2) ? std::max(chips[0], chips[1]) : -1;
    }
};

InstructionPtr ParseInstruction(const std::string& text) {
    std::istringstream iss(text);
    std::vector<std::string> tokens;
    std::copy(std::istream_iterator<std::string>(iss),
              std::istream_iterator<std::string>(), std::back_inserter(tokens));

    if ("value" == tokens.front()) {
        InputInstructionPtr inst = std::make_shared<InputInstruction>();
        inst->type = InstructionType::kInput;
        inst->dst_bot = std::stoi(tokens[5]);
        inst->value = std::stoi(tokens[1]);

        return inst;
    } else {
        OutputInstructionPtr inst = std::make_shared<OutputInstruction>();
        inst->type = InstructionType::kOutput;
        inst->src_bot = std::stoi(tokens[1]);
        inst->low_target = tokens[5];
        inst->low_id = std::stoi(tokens[6]);
        inst->high_target = tokens[10];
        inst->high_id = std::stoi(tokens[11]);

        return inst;
    }
}

int GetTargetBot(const std::unordered_map<int, Bot>& robots, int target_low,
                 int target_high) {
    for (const auto& [bot_id, robot] : robots) {
        if ((robot.GetLow() == target_low) &&
            (robot.GetHigh() == target_high)) {
            return bot_id;
        }
    }
    return -1;
}

int GetBotId(const std::vector<InstructionPtr>& instructions, int target_low,
             int target_high) {
    std::unordered_map<int, Bot> robots;
    InputInstructionPtr input_inst = nullptr;
    OutputInstructionPtr output_inst = nullptr;
    int target_bot = -1;
    while (-1 == (target_bot = GetTargetBot(robots, target_low, target_high))) {
        for (const InstructionPtr& inst : instructions) {
            switch (inst->type) {
                case InstructionType::kInput:
                    input_inst =
                        std::dynamic_pointer_cast<InputInstruction>(inst);
                    robots[input_inst->dst_bot].InsertChip(input_inst->value);
                    break;
                case InstructionType::kOutput:
                    output_inst =
                        std::dynamic_pointer_cast<OutputInstruction>(inst);
                    if ((output_inst->low_target != "output") &&
                        (robots[output_inst->src_bot].GetLow() != -1)) {
                        robots[output_inst->low_id].InsertChip(
                            robots[output_inst->src_bot].GetLow());
                    }
                    if ((output_inst->high_target != "output") &&
                        (robots[output_inst->src_bot].GetHigh() != -1)) {
                        robots[output_inst->high_id].InsertChip(
                            robots[output_inst->src_bot].GetHigh());
                    }
                    break;
            }
        }
    }
    return target_bot;
}

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "error: missing puzzle input file" << std::endl;
        exit(EXIT_FAILURE);
    }

    std::ifstream input(argv[1]);
    std::string instruction;
    std::vector<InstructionPtr> instructions;
    while (std::getline(input, instruction)) {
        instructions.push_back(ParseInstruction(instruction));
    }

    const int kTargetLow = 17;
    const int kTargetHigh = 61;
    std::cout << "Answer: " << GetBotId(instructions, kTargetLow, kTargetHigh)
              << std::endl;

    exit(EXIT_SUCCESS);
}
