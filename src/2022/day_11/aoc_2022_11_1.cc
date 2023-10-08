#include <algorithm>
#include <climits>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <queue>
#include <regex>
#include <string>

enum Operator { kMultiply, kAdd };

class Operation {
 public:
  Operation()
      : operation_(Operator::kAdd),
        operand_1_(kUninitializedOperand),
        operand_2_(kUninitializedOperand),
        requires_double_operand_(false) {}

  Operation(Operator operation, int operand_1)
      : operation_(operation),
        operand_1_(operand_1),
        operand_2_(kUninitializedOperand),
        requires_double_operand_(false) {}

  Operation(Operator operation)
      : operation_(operation),
        operand_1_(kUninitializedOperand),
        operand_2_(kUninitializedOperand),
        requires_double_operand_(true) {}

  int Run(int operand) {
    operand_2_ = operand;
    return ExecOperation();
  }

  int Run(int operand_1, int operand_2) {
    operand_1_ = operand_1;
    operand_2_ = operand_2;
    return ExecOperation();
  }

  bool RequiresDoubleOperand() const { return requires_double_operand_; }

 private:
  static const int kUninitializedOperand = INT_MIN;

  int ExecOperation() const {
    switch (operation_) {
      case Operator::kMultiply:
        return operand_1_ * operand_2_;
      case Operator::kAdd:
        return operand_1_ + operand_2_;
      default:
        return kUninitializedOperand;
    }
  }

  Operator operation_;
  int operand_1_;
  int operand_2_;
  bool requires_double_operand_;
};

class MonkeyTest {
 public:
  MonkeyTest() : divisor_(1), monkey_on_true_(-1), monkey_on_false_(-1) {}

  MonkeyTest(int divisor, int monkey_on_true, int monkey_on_false)
      : divisor_(divisor),
        monkey_on_true_(monkey_on_true),
        monkey_on_false_(monkey_on_false) {}

  int Run(int worry_level) const {
    return (0 == (worry_level % divisor_)) ? monkey_on_true_ : monkey_on_false_;
  }

 private:
  int divisor_;
  int monkey_on_true_;
  int monkey_on_false_;
};

struct Monkey {
  std::queue<int> worry_levels;
  Operation operation;
  MonkeyTest test;
  int inspection_count;
};

int CalculateMonkeyBusiness(std::vector<Monkey>& monkeys) {
  // Sort monkeys by inspection count so that the two monkeys with the
  // largest number of inspections are at the back of the list.
  std::sort(monkeys.begin(), monkeys.end(),
            [](const Monkey& m1, const Monkey& m2) {
              return (m1.inspection_count < m2.inspection_count);
            });

  // Compute the monkey business value.
  int num_monkeys = monkeys.size();
  int monkey_business = monkeys[num_monkeys - 1].inspection_count *
                        monkeys[num_monkeys - 2].inspection_count;

  return monkey_business;
}

int SimulateMonkeyBusiness(std::vector<Monkey>& monkeys, int num_rounds) {
  while (num_rounds--) {
    for (Monkey& monkey : monkeys) {
      int num_items = monkey.worry_levels.size();
      for (int i = 0; i < num_items; ++i) {
        int curr_worry_level = monkey.worry_levels.front();
        monkey.worry_levels.pop();

        // Apply the worry level change caused by monkey inspection.
        int new_worry_level = 0;
        if (monkey.operation.RequiresDoubleOperand()) {
          new_worry_level =
              monkey.operation.Run(curr_worry_level, curr_worry_level);
        } else {
          new_worry_level = monkey.operation.Run(curr_worry_level);
        }

        // Apply the 'relaxation' factor.
        new_worry_level /= 3;

        // Determine the next monkey to have this item and pass them
        // the item.
        int next_monkey = monkey.test.Run(new_worry_level);
        monkeys[next_monkey].worry_levels.push(new_worry_level);

        // Record this inspection.
        monkey.inspection_count++;
      }
    }
  }

  return CalculateMonkeyBusiness(monkeys);
}

std::queue<int> ParseItems(const std::string& input) {
  std::queue<int> items;
  std::regex re("[\\d+]+");

  std::sregex_iterator iter(input.begin(), input.end(), re);
  std::sregex_iterator end;
  while (iter != end) {
    items.push(std::stoi((*iter)[0]));
    ++iter;
  }
  return items;
}

Operation ParseOperation(const std::string& input) {
  // This regex search extracts the expression following the '=' character
  // in the input line.
  std::regex expression_re(".*=\\s*(.*)");
  std::smatch matches;
  std::regex_search(input, matches, expression_re);

  // This regex tokenizes the expression by splitting it on whitespace.
  std::string expression(matches[1]);
  std::regex word_re("\\s+");
  std::sregex_token_iterator iter(expression.begin(), expression.end(), word_re,
                                  -1);
  std::sregex_token_iterator end;
  std::vector<std::string> tokens;
  for (; iter != end; ++iter) tokens.push_back(*iter);

  Operator op = (tokens[1] == "+") ? Operator::kAdd : Operator::kMultiply;
  static const std::string kOld = "old";
  if ((tokens[0] == kOld) && (tokens[2] == kOld)) {
    return Operation(op);
  }
  return Operation(op, std::stoi(tokens[2]));
}

MonkeyTest ParseMonkeyTest(const std::string& condition,
                           const std::string& if_true,
                           const std::string& if_false) {
  std::regex num_re("(\\d+)");
  std::smatch matches;

  int divisor = 0;
  if (std::regex_search(condition, matches, num_re))
    divisor = std::stoi(matches[1]);

  int monkey_if_true = 0;
  if (std::regex_search(if_true, matches, num_re))
    monkey_if_true = std::stoi(matches[1]);

  int monkey_if_false = 0;
  if (std::regex_search(if_false, matches, num_re))
    monkey_if_false = std::stoi(matches[1]);

  return MonkeyTest(divisor, monkey_if_true, monkey_if_false);
}

std::vector<Monkey> LoadMonkeys(const std::string& input) {
  std::ifstream monkey_is(input);

  std::vector<Monkey> monkeys;
  std::string line;
  int curr_line = 0;

  Operation op;
  MonkeyTest test;
  std::queue<int> worry_levels;
  while (std::getline(monkey_is, line)) {
    if (curr_line == 6) {
      Monkey m;
      m.worry_levels = worry_levels;
      m.operation = op;
      m.test = test;
      m.inspection_count = 0;
      monkeys.push_back(m);

      curr_line = 0;
      continue;
    }

    switch (curr_line) {
      case 0:
        // Noop on first line, we don't do anything with it.
        curr_line++;
        break;
      case 1:
        worry_levels = ParseItems(line);
        curr_line++;
        break;
      case 2:
        op = ParseOperation(line);
        curr_line++;
        break;
      case 3:
        std::vector<std::string> test_lines = {line};
        curr_line++;
        while (curr_line < 6) {
          std::getline(monkey_is, line);
          test_lines.push_back(line);
          curr_line++;
        }
        test = ParseMonkeyTest(test_lines[0], test_lines[1], test_lines[2]);
        break;
    }
  }

  return monkeys;
}

int main(int argc, char** argv) {
  if (argc != 2) {
    std::cerr << "usage: aoc INPUT_FILE" << std::endl;
    exit(EXIT_FAILURE);
  }

  const int kNumRounds = 20;
  std::vector<Monkey> monkeys = LoadMonkeys(argv[1]);
  std::cout << "Monkey Business Value: "
            << SimulateMonkeyBusiness(monkeys, kNumRounds) << std::endl;

  exit(EXIT_SUCCESS);
}
