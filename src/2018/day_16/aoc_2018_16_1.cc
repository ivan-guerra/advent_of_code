#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>
#include <vector>

using Registers = std::vector<int>;

enum class OpCode {
  kAddr,
  kAddi,
  kMulr,
  kMuli,
  kBanr,
  kBani,
  kBorr,
  kBori,
  kSetr,
  kSeti,
  kGtir,
  kGtri,
  kGtrr,
  kEqir,
  kEqri,
  kEqrr,
  kUnknown
};

struct Instruction {
  OpCode opcode;
  int arg1;
  int arg2;
  int dst;
};

struct Sample {
  Registers before;
  Instruction inst;
  Registers after;
};

std::vector<std::string> GetWords(const std::string& text) {
  std::istringstream iss(text);
  std::vector<std::string> words;
  std::copy(std::istream_iterator<std::string>(iss),
            std::istream_iterator<std::string>(), std::back_inserter(words));
  return words;
}

Registers ParseRegisterList(const std::string& list) {
  Registers registers;
  std::vector<std::string> tokens = GetWords(list);
  registers.push_back(std::stoi(tokens[1].substr(1, tokens[1].size() - 1)));
  for (int i = 2; i < tokens.size(); ++i) {
    registers.push_back(std::stoi(tokens[i].substr(0, tokens[i].size() - 1)));
  }
  return registers;
}

Instruction ParseInstruction(const std::string& inst_str) {
  std::vector<std::string> tokens = GetWords(inst_str);
  Instruction inst = {
      .opcode = OpCode::kUnknown,
      .arg1 = std::stoi(tokens[1]),
      .arg2 = std::stoi(tokens[2]),
      .dst = std::stoi(tokens[3]),
  };
  return inst;
}

std::vector<Sample> ParseSamples(const std::string& file) {
  std::ifstream input(file);
  std::string line1, line2, line3, line4;

  std::vector<Sample> samples;
  while (std::getline(input, line1)) {
    if (line1.empty()) {
      /* Break before getting to the program instructions... */
      break;
    }
    std::getline(input, line2);
    std::getline(input, line3);
    Sample sample = {
        .before = ParseRegisterList(line1),
        .inst = ParseInstruction(line2),
        .after = ParseRegisterList(line3),
    };
    samples.push_back(sample);
    std::getline(input, line4);  // Consume blank line.
  }
  return samples;
}

Registers ExecInstruction(const Instruction& inst, const Registers& init_reg) {
  Registers out_registers = init_reg;
  switch (inst.opcode) {
    case OpCode::kAddr:
      out_registers[inst.dst] = init_reg[inst.arg1] + init_reg[inst.arg2];
      break;
    case OpCode::kAddi:
      out_registers[inst.dst] = init_reg[inst.arg1] + inst.arg2;
      break;
    case OpCode::kMulr:
      out_registers[inst.dst] = init_reg[inst.arg1] * init_reg[inst.arg2];
      break;
    case OpCode::kMuli:
      out_registers[inst.dst] = init_reg[inst.arg1] * inst.arg2;
      break;
    case OpCode::kBanr:
      out_registers[inst.dst] = init_reg[inst.arg1] & init_reg[inst.arg2];
      break;
    case OpCode::kBani:
      out_registers[inst.dst] = init_reg[inst.arg1] & inst.arg2;
      break;
    case OpCode::kBorr:
      out_registers[inst.dst] = init_reg[inst.arg1] | init_reg[inst.arg2];
      break;
    case OpCode::kBori:
      out_registers[inst.dst] = init_reg[inst.arg1] | inst.arg2;
      break;
    case OpCode::kSetr:
      out_registers[inst.dst] = init_reg[inst.arg1];
      break;
    case OpCode::kSeti:
      out_registers[inst.dst] = inst.arg1;
      break;
    case OpCode::kGtir:
      out_registers[inst.dst] = (inst.arg1 > init_reg[inst.arg2]) ? 1 : 0;
      break;
    case OpCode::kGtri:
      out_registers[inst.dst] = (init_reg[inst.arg1] > inst.arg2) ? 1 : 0;
      break;
    case OpCode::kGtrr:
      out_registers[inst.dst] =
          (init_reg[inst.arg1] > init_reg[inst.arg2]) ? 1 : 0;
      break;
    case OpCode::kEqir:
      out_registers[inst.dst] = (inst.arg1 == init_reg[inst.arg2]) ? 1 : 0;
      break;
    case OpCode::kEqri:
      out_registers[inst.dst] = (init_reg[inst.arg1] == inst.arg2) ? 1 : 0;
      break;
    case OpCode::kEqrr:
      out_registers[inst.dst] =
          (init_reg[inst.arg1] == init_reg[inst.arg2]) ? 1 : 0;
      break;
    case OpCode::kUnknown:
      std::cerr << "error: unknown opcode detected" << std::endl;
      break;
  }
  return out_registers;
}

bool IsPossibleOp(const Sample& sample) {
  return (sample.after == ExecInstruction(sample.inst, sample.before));
}

int NumCandidateSamples(std::vector<Sample>& samples, int threshold = 3) {
  static const std::vector<OpCode> kOpCodes = {
      OpCode::kAddr, OpCode::kAddi, OpCode::kMulr, OpCode::kMuli,
      OpCode::kBanr, OpCode::kBani, OpCode::kBorr, OpCode::kBori,
      OpCode::kSetr, OpCode::kSeti, OpCode::kGtir, OpCode::kGtri,
      OpCode::kGtrr, OpCode::kEqir, OpCode::kEqri, OpCode::kEqrr,
  };

  int total_candidates = 0;
  for (Sample& sample : samples) {
    int matches =
        std::count_if(kOpCodes.begin(), kOpCodes.end(), [&](OpCode opcode) {
          sample.inst.opcode = opcode;
          return IsPossibleOp(sample);
        });
    if (matches >= threshold) {
      total_candidates++;
    }
  }
  return total_candidates;
}

int main(int argc, char** argv) {
  if (argc != 2) {
    std::cerr << "error: missing puzzle input file" << std::endl;
    exit(EXIT_FAILURE);
  }

  std::vector<Sample> samples = ParseSamples(argv[1]);

  std::cout << "Answer: " << NumCandidateSamples(samples) << std::endl;

  exit(EXIT_SUCCESS);
}
