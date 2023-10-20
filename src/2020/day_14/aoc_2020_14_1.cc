#include <bitset>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

static constexpr int kAddrSpaceSize = 36;
using Value = std::bitset<kAddrSpaceSize>;

enum class BitState {
  kHigh,
  kLow,
  kNop,
};

using Mask = std::vector<BitState>;

enum class InstructionType {
  kSetMask,
  kUpdateMem,
};

struct Instruction {
  InstructionType type;
  Mask mask;
  uint64_t addr = 0;
  uint64_t value = 0;
};

using Memory = std::unordered_map<uint64_t, uint64_t>;

Instruction ParseMaskInstruction(const std::string& raw_mask) {
  Mask mask;
  int equals_idx = raw_mask.find('=');
  std::string mask_str = raw_mask.substr(equals_idx + 2);
  for (int i = mask_str.size() - 1; i >= 0; --i) {
    if (mask_str[i] == 'X') {
      mask.push_back(BitState::kNop);
    } else if (mask_str[i] == '1') {
      mask.push_back(BitState::kHigh);
    } else {
      mask.push_back(BitState::kLow);
    }
  }
  return {
      .type = InstructionType::kSetMask, .mask = mask, .addr = 0, .value = 0};
}

Instruction ParseMemInstruction(const std::string& raw_instruction) {
  int bracket_open = raw_instruction.find('[');
  int bracket_close = raw_instruction.find(']');
  uint64_t addr = std::stoull(raw_instruction.substr(
      bracket_open + 1, bracket_close - bracket_open - 1));

  int equals_idx = raw_instruction.find('=');
  uint64_t value = std::stoull(raw_instruction.substr(equals_idx + 2));

  return {.type = InstructionType::kUpdateMem,
          .mask = {},
          .addr = addr,
          .value = value};
}

std::vector<Instruction> ParseInstructions(const std::string& infile) {
  std::fstream fhandle(infile);
  std::string line;
  std::vector<Instruction> instructions;
  while (std::getline(fhandle, line)) {
    if (line.find("mem") != std::string::npos) {
      instructions.push_back(ParseMemInstruction(line));
    } else {
      instructions.push_back(ParseMaskInstruction(line));
    }
  }
  return instructions;
}

Value ValueToBitset(uint64_t val) {
  Value bits;
  for (int i = 0; i < bits.size(); ++i) {
    bits[i] = (val & 1) ? 1 : 0;
    val >>= 1;
  }
  return bits;
}

Value ApplyMask(const Value& value, const Mask& mask) {
  Value result = value;
  for (int i = 0; i < value.size(); ++i) {
    switch (mask[i]) {
      case BitState::kHigh:
        result[i] = 1;
        break;
      case BitState::kLow:
        result[i] = 0;
        break;
      case BitState::kNop:
        break;
    }
  }
  return result;
}

void ExecuteInstructions(const std::vector<Instruction>& instructions,
                         Memory& mem) {
  Mask curr_mask;
  for (const Instruction& inst : instructions) {
    switch (inst.type) {
      case InstructionType::kSetMask:
        curr_mask = inst.mask;
        break;
      case InstructionType::kUpdateMem:
        mem[inst.addr] =
            ApplyMask(ValueToBitset(inst.value), curr_mask).to_ullong();
        break;
    }
  }
}

int main(int argc, char** argv) {
  if (argc != 2) {
    std::cerr << "error: missing puzzle input file" << std::endl;
    exit(EXIT_FAILURE);
  }

  Memory mem;
  std::vector<Instruction> instructions = ParseInstructions(argv[1]);
  ExecuteInstructions(instructions, mem);

  uint64_t sum = 0ULL;
  for (const auto& [addr, value] : mem) {
    sum += value;
  }
  std::cout << "Answer: " << sum << std::endl;

  exit(EXIT_SUCCESS);
}
