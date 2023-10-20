#include <algorithm>
#include <bitset>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <iterator>
#include <string>
#include <unordered_map>
#include <vector>

static constexpr int kAddrSpaceSize = 36;

enum class BitState {
  kHigh,
  kLow,
  kFloating,
};

using BitStateVec = std::vector<BitState>;

enum class InstructionType {
  kSetMask,
  kUpdateMem,
};

struct Instruction {
  InstructionType type;
  BitStateVec mask;
  BitStateVec addr;
  uint64_t value;
};

using Memory = std::unordered_map<uint64_t, uint64_t>;

Instruction ParseMaskInstruction(const std::string& raw_mask) {
  BitStateVec mask;
  int equals_idx = raw_mask.find('=');
  std::string mask_str = raw_mask.substr(equals_idx + 2);
  for (int i = mask_str.size() - 1; i >= 0; --i) {
    if (mask_str[i] == 'X') {
      mask.push_back(BitState::kFloating);
    } else if (mask_str[i] == '1') {
      mask.push_back(BitState::kHigh);
    } else {
      mask.push_back(BitState::kLow);
    }
  }
  return {
      .type = InstructionType::kSetMask, .mask = mask, .addr = {}, .value = 0};
}

Instruction ParseMemInstruction(const std::string& raw_instruction) {
  int bracket_open = raw_instruction.find('[');
  int bracket_close = raw_instruction.find(']');
  uint64_t addr = std::stoull(raw_instruction.substr(
      bracket_open + 1, bracket_close - bracket_open - 1));

  BitStateVec addr_vec;
  for (int i = 0; i < kAddrSpaceSize; ++i) {
    addr_vec.push_back((addr & 1) ? BitState::kHigh : BitState::kLow);
    addr >>= 1;
  }

  int equals_idx = raw_instruction.find('=');
  uint64_t value = std::stoull(raw_instruction.substr(equals_idx + 2));

  return {.type = InstructionType::kUpdateMem,
          .mask = {},
          .addr = addr_vec,
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

BitStateVec ComputeMaskedAddr(const BitStateVec& addr,
                              const BitStateVec& mask) {
  BitStateVec masked_addr = addr;
  for (int i = 0; i < addr.size(); ++i) {
    switch (mask[i]) {
      case BitState::kHigh:
        masked_addr[i] = BitState::kHigh;
        break;
      case BitState::kLow:
        /* Leave bit state unchanged. */
        break;
      case BitState::kFloating:
        masked_addr[i] = BitState::kFloating;
        break;
    }
  }
  return masked_addr;
}

void GenerateAddresses(std::vector<uint64_t>& addresses,
                       const BitStateVec& states, int pos, uint64_t addr) {
  if (pos == kAddrSpaceSize) {
    addresses.push_back(addr);
    return;
  }

  switch (states[pos]) {
    case BitState::kLow:
      addr <<= 1;
      GenerateAddresses(addresses, states, pos + 1, addr);
      break;
    case BitState::kHigh:
      addr = (addr << 1) | 1ULL;
      GenerateAddresses(addresses, states, pos + 1, addr);
      break;
    case BitState::kFloating:
      addr <<= 1;
      GenerateAddresses(addresses, states, pos + 1, addr);
      addr |= 1ULL;
      GenerateAddresses(addresses, states, pos + 1, addr);
      break;
  }
}

void ExecuteInstructions(const std::vector<Instruction>& instructions,
                         Memory& mem) {
  BitStateVec curr_mask;
  for (const Instruction& inst : instructions) {
    switch (inst.type) {
      case InstructionType::kSetMask:
        curr_mask = inst.mask;
        break;
      case InstructionType::kUpdateMem:
        BitStateVec masked_addr = ComputeMaskedAddr(inst.addr, curr_mask);
        std::reverse(std::begin(masked_addr), std::end(masked_addr));

        std::vector<uint64_t> addresses;
        GenerateAddresses(addresses, masked_addr, 0, 0);

        for (const uint64_t& addr : addresses) {
          mem[addr] = inst.value;
        }
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
