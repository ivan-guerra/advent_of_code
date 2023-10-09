#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using ExpenseList = std::vector<uint32_t>;

ExpenseList LoadExpenses(const std::string& expense_file) {
  std::ifstream expense_report(expense_file);
  std::string entry;
  ExpenseList expenses;
  while (std::getline(expense_report, entry)) {
    expenses.emplace_back(std::stoi(entry));
  }
  return expenses;
}

std::pair<int, int> FindTargetSum(const ExpenseList& expenses, int target) {
  for (int i = 0; i < expenses.size(); ++i) {
    for (int j = i + 1; j < expenses.size(); ++j) {
      if (expenses[i] + expenses[j] == target) {
        return {expenses[i], expenses[j]};
      }
    }
  }
  return {};
}

int main(int argc, char** argv) {
  if (argc != 2) {
    std::cerr << "error: missing puzzle input file" << std::endl;
    exit(EXIT_FAILURE);
  }

  const int kTargetSum = 2020;
  ExpenseList expenses = LoadExpenses(argv[1]);
  std::pair<int, int> target_pair = FindTargetSum(expenses, kTargetSum);
  std::cout << target_pair.first * target_pair.second << std::endl;

  exit(EXIT_SUCCESS);
}
