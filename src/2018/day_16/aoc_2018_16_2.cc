#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

enum OpCode : int;
struct Sample;
struct Instruction;

using Registers = std::vector<int>;
using SampleList = std::vector<Sample>;
using InstructionList = std::vector<Instruction>;
using CandidateMap = std::unordered_map<OpCode, std::unordered_set<int>>;
using DecoderMap = std::unordered_map<int, OpCode>;

enum OpCode : int {
  kAddr = 0,
  kAddi = 1,
  kMulr = 2,
  kMuli = 3,
  kBanr = 4,
  kBani = 5,
  kBorr = 6,
  kBori = 7,
  kSetr = 8,
  kSeti = 9,
  kGtir = 10,
  kGtri = 11,
  kGtrr = 12,
  kEqir = 13,
  kEqri = 14,
  kEqrr = 15,
  kUnknown = 16,
};

struct Instruction {
  OpCode opcode;
  int candidate_opcode;
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
      .candidate_opcode = std::stoi(tokens[0]),
      .arg1 = std::stoi(tokens[1]),
      .arg2 = std::stoi(tokens[2]),
      .dst = std::stoi(tokens[3]),
  };
  return inst;
}

SampleList ParseSamples(const std::string& file) {
  std::ifstream input(file);
  std::string line1, line2, line3, line4;

  SampleList samples;
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

InstructionList ParseInstructions(const std::string& file) {
  std::ifstream input(file);
  std::string curr_line;
  std::string prev_line;
  std::getline(input, curr_line);
  while (curr_line != prev_line) {
    prev_line = curr_line;
    std::getline(input, curr_line);
  }
  std::getline(input, curr_line);

  InstructionList instructions;
  while (std::getline(input, curr_line)) {
    instructions.push_back(ParseInstruction(curr_line));
  }
  return instructions;
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

CandidateMap GetCandidateOpCodeToValueMappings(
    SampleList& samples, const std::unordered_set<OpCode>& candidate_ops,
    const std::unordered_set<int>& assigned_codes) {
  CandidateMap op_map;
  for (const OpCode& opcode : candidate_ops) {
    for (Sample& sample : samples) {
      /* Ignore code values that have previously been assigned. */
      if (assigned_codes.count(sample.inst.candidate_opcode)) {
        continue;
      }

      sample.inst.opcode = opcode;
      if (IsPossibleOp(sample)) {
        op_map[opcode].insert(sample.inst.candidate_opcode);
      }
    }
  }
  return op_map;
}

DecoderMap GetDecoderMap(SampleList& samples) {
  std::unordered_map<int, OpCode> decoder;
  std::unordered_set<int> assigned_codes;
  std::unordered_set<OpCode> candidate_ops = {
      OpCode::kAddr, OpCode::kAddi, OpCode::kMulr, OpCode::kMuli,
      OpCode::kBanr, OpCode::kBani, OpCode::kBorr, OpCode::kBori,
      OpCode::kSetr, OpCode::kSeti, OpCode::kGtir, OpCode::kGtri,
      OpCode::kGtrr, OpCode::kEqir, OpCode::kEqri, OpCode::kEqrr,
  };

  while (!candidate_ops.empty()) {
    CandidateMap candidates = GetCandidateOpCodeToValueMappings(
        samples, candidate_ops, assigned_codes);
    for (const auto& [opcode, codes] : candidates) {
      /* There's only one possible opcode that can match this value! */
      if (codes.size() == 1) {
        decoder[*codes.begin()] = opcode;
        assigned_codes.insert(*codes.begin());
        candidate_ops.erase(opcode);
      }
    }
  }
  return decoder;
}

int ExecProgram(InstructionList& instructions, const DecoderMap& decoder) {
  std::vector<int> registers(4, 0);
  for (Instruction& inst : instructions) {
    inst.opcode = decoder.at(inst.candidate_opcode);
    registers = ExecInstruction(inst, registers);
  }
  return registers[0];
}

int main(int argc, char** argv) {
  if (argc != 2) {
    std::cerr << "error: missing puzzle input file" << std::endl;
    exit(EXIT_FAILURE);
  }

  SampleList samples = ParseSamples(argv[1]);
  DecoderMap decoder = GetDecoderMap(samples);
  InstructionList instructions = ParseInstructions(argv[1]);

  std::cout << "Answer: " << ExecProgram(instructions, decoder) << std::endl;

  exit(EXIT_SUCCESS);
}
