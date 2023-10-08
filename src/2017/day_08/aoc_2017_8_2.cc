#include <algorithm>
#include <climits>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

using RegisterMap = std::unordered_map<std::string, int>;

enum InstructionType {
  kInc,
  kDec,
};

enum Operator {
  kEquals,
  kNotEqual,
  kLess,
  kGreater,
  kLeq,
  kGeq,
};

struct Instruction {
  InstructionType type;
  Operator cond_op;
  std::string operand1;
  std::string operand2;
  std::string dst_register;
  int delta = 0;
};

Instruction ParseInstruction(const std::string text) {
  std::istringstream iss(text);
  std::vector<std::string> tokens;
  std::copy(std::istream_iterator<std::string>(iss),
            std::istream_iterator<std::string>(), std::back_inserter(tokens));

  Instruction inst;
  inst.type =
      (tokens[1] == "inc") ? InstructionType::kInc : InstructionType::kDec;
  if (tokens[5] == "==") {
    inst.cond_op = Operator::kEquals;
  } else if (tokens[5] == "!=") {
    inst.cond_op = Operator::kNotEqual;
  } else if (tokens[5] == "<") {
    inst.cond_op = Operator::kLess;
  } else if (tokens[5] == ">") {
    inst.cond_op = Operator::kGreater;
  } else if (tokens[5] == "<=") {
    inst.cond_op = Operator::kLeq;
  } else if (tokens[5] == ">=") {
    inst.cond_op = Operator::kGeq;
  }
  inst.operand1 = tokens[4];
  inst.operand2 = tokens[6];
  inst.dst_register = tokens[0];
  inst.delta = std::stoi(tokens[2]);

  return inst;
}

bool IsRegister(const std::string& label) {
  return std::any_of(label.begin(), label.end(),
                     [](char a) { return std::isalpha(a); });
}

int ResolveValue(const RegisterMap& registers, const std::string& operand) {
  if (IsRegister(operand)) {
    return (registers.count(operand)) ? registers.at(operand) : 0;
  }
  return std::stoi(operand);
}

bool EvaluateConditional(RegisterMap& registers, const std::string& operand1,
                         const std::string& operand2, const Operator& op) {
  int op1_val = ResolveValue(registers, operand1);
  int op2_val = ResolveValue(registers, operand2);
  switch (op) {
    case kEquals:
      return (op1_val == op2_val);
    case kNotEqual:
      return (op1_val != op2_val);
    case kLess:
      return (op1_val < op2_val);
    case kGreater:
      return (op1_val > op2_val);
    case kLeq:
      return (op1_val <= op2_val);
    case kGeq:
      return (op1_val >= op2_val);
  }
  return false; /* unreachable */
}

int ComputeHighestHistoricalVal(const std::vector<Instruction>& instructions) {
  int max_val = INT_MIN;
  RegisterMap registers;
  for (const Instruction& inst : instructions) {
    if (EvaluateConditional(registers, inst.operand1, inst.operand2,
                            inst.cond_op)) {
      int delta = 0;
      if (inst.type == InstructionType::kDec) {
        delta = -inst.delta;
      } else {
        delta = inst.delta;
      }

      if (registers.count(inst.dst_register)) {
        registers[inst.dst_register] += delta;
      } else {
        registers[inst.dst_register] = delta;
      }
      max_val = std::max(max_val, registers[inst.dst_register]);
    }
  }
  return max_val;
}

int main(int argc, char** argv) {
  if (argc != 2) {
    std::cerr << "error: missing puzzle input file" << std::endl;
    exit(EXIT_FAILURE);
  }

  std::ifstream input(argv[1]);
  std::string text;
  std::vector<Instruction> instructions;
  while (std::getline(input, text)) {
    instructions.push_back(ParseInstruction(text));
  }

  std::cout << "Answer: " << ComputeHighestHistoricalVal(instructions)
            << std::endl;

  exit(EXIT_SUCCESS);
}
