#include <algorithm>
#include <cstdint>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <iterator>
#include <map>
#include <queue>
#include <sstream>
#include <vector>

enum Operator {
  kAssign,
  kAnd,
  kOr,
  kLShift,
  kRShift,
  kComplement,
  kNone,
};

enum OperandType {
  kVariable,
  kLiteral,
};

struct Operand {
  OperandType type;
  std::string lexeme;
};

struct Instruction {
  Operator op = Operator::kNone;
  std::vector<Operand> operands;
  std::string output;
};

bool IsVariable(const std::string& lexeme) {
  return std::all_of(lexeme.begin(), lexeme.end(),
                     [](char c) { return std::isalpha(c); });
}

Operand GetOperand(const std::string& lexeme) {
  Operand operand;
  operand.type =
      (IsVariable(lexeme)) ? OperandType::kVariable : OperandType::kLiteral;
  operand.lexeme = lexeme;
  return operand;
}

Instruction ParseInstruction(const std::string& instruction) {
  std::istringstream iss(instruction);
  std::vector<std::string> tokens;
  std::copy(std::istream_iterator<std::string>(iss),
            std::istream_iterator<std::string>(), std::back_inserter(tokens));

  Instruction inst;
  Operand operand;
  if (tokens.size() == 3) { /* Assignment */
    inst.op = Operator::kAssign;
    inst.operands.push_back(GetOperand(tokens[0]));
    inst.output = tokens[2];
  } else if (tokens.size() == 4) { /* Bitwise Complement */
    inst.op = Operator::kComplement;
    inst.operands.push_back(GetOperand(tokens[1]));
    inst.output = tokens[3];
  } else if (tokens.size() == 5) { /* Binary operator. */
    if ("AND" == tokens[1]) {
      inst.op = Operator::kAnd;
    } else if ("OR" == tokens[1]) {
      inst.op = Operator::kOr;
    } else if ("LSHIFT" == tokens[1]) {
      inst.op = Operator::kLShift;
    } else if ("RSHIFT" == tokens[1]) {
      inst.op = Operator::kRShift;
    }
    inst.operands.push_back(GetOperand(tokens[0]));
    inst.operands.push_back(GetOperand(tokens[2]));
    inst.output = tokens[4];
  }
  return inst;
}

uint16_t EvaluateOperand(const Operand& operand,
                         std::map<std::string, uint16_t>& wires) {
  /* Override of value b to value of signal a in Part I. */
  if ((operand.type == OperandType::kVariable) && (operand.lexeme == "b")) {
    return 3176;
  }
  return (operand.type == OperandType::kVariable) ? wires[operand.lexeme]
                                                  : std::stoi(operand.lexeme);
}

void ExecuteInstruction(const Instruction& inst,
                        std::map<std::string, uint16_t>& wires) {
  switch (inst.op) {
    case Operator::kAssign:
      wires[inst.output] = EvaluateOperand(inst.operands[0], wires);
      break;
    case Operator::kAnd:
      wires[inst.output] = EvaluateOperand(inst.operands[0], wires) &
                           EvaluateOperand(inst.operands[1], wires);
      break;
    case Operator::kOr:
      wires[inst.output] = EvaluateOperand(inst.operands[0], wires) |
                           EvaluateOperand(inst.operands[1], wires);
      break;
    case Operator::kLShift:
      wires[inst.output] = EvaluateOperand(inst.operands[0], wires)
                           << EvaluateOperand(inst.operands[1], wires);
      break;
    case Operator::kRShift:
      wires[inst.output] = EvaluateOperand(inst.operands[0], wires) >>
                           EvaluateOperand(inst.operands[1], wires);
      break;
    case Operator::kComplement:
      wires[inst.output] = ~EvaluateOperand(inst.operands[0], wires);
      break;
    case Operator::kNone:
    default:
      std::cerr << "error: unknown operation" << std::endl;
      exit(EXIT_FAILURE);
  }
}

bool CanExecuteInstruction(const Instruction& inst,
                           std::map<std::string, uint16_t>& wires) {
  for (const Operand& operand : inst.operands) {
    if ((OperandType::kVariable == operand.type) &&
        (wires.find(operand.lexeme) == wires.end())) {
      return false;
    }
  }
  return true;
}

void ExecuteInstructions(std::queue<Instruction>& instructions,
                         std::map<std::string, uint16_t>& wires) {
  while (!instructions.empty()) {
    Instruction inst = instructions.front();
    instructions.pop();
    if (CanExecuteInstruction(inst, wires)) {
      ExecuteInstruction(inst, wires);
    } else {
      instructions.push(inst);
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

  std::map<std::string, uint16_t> wires;
  std::queue<Instruction> instructions;
  while (std::getline(input, instruction)) {
    instructions.push(ParseInstruction(instruction));
  }
  ExecuteInstructions(instructions, wires);

  std::cout << "Answer: " << wires["a"] << std::endl;

  exit(EXIT_SUCCESS);
}
