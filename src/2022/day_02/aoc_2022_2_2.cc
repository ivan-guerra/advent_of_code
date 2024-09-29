#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>

enum Hand { kRock = 1, kPaper = 2, kScissor = 3 };
enum Outcome { kWin = 6, kDraw = 3, kLose = 0 };

int GetScore(const std::string input) {
  static const std::unordered_map<Hand, Hand> kWinningMoves = {
      {Hand::kRock, Hand::kScissor},
      {Hand::kPaper, Hand::kRock},
      {Hand::kScissor, Hand::kPaper}};
  static const std::unordered_map<Hand, Hand> kLosingMoves = {
      {Hand::kScissor, Hand::kRock},
      {Hand::kRock, Hand::kPaper},
      {Hand::kPaper, Hand::kScissor}};
  static const std::unordered_map<char, Hand> kOpponentHand = {
      {'A', Hand::kRock}, {'B', Hand::kPaper}, {'C', Hand::kScissor}};
  static const std::unordered_map<char, Outcome> kExpectedOutcomes = {
      {'X', Outcome::kLose}, {'Y', Outcome::kDraw}, {'Z', Outcome::kWin}};

  std::ifstream strategy(input);

  int score = 0;
  std::string line;
  while (std::getline(strategy, line)) {
    Hand opponent_hand = kOpponentHand.at(line[0]);
    Outcome expected_outcome = kExpectedOutcomes.at(line[2]);
    switch (expected_outcome) {
      case Outcome::kDraw:
        score +=
            static_cast<int>(opponent_hand) + static_cast<int>(Outcome::kDraw);
        break;
      case Outcome::kWin:
        score += static_cast<int>(kLosingMoves.at(opponent_hand)) +
                 static_cast<int>(Outcome::kWin);
        break;
      case Outcome::kLose:
        score += static_cast<int>(kWinningMoves.at(opponent_hand)) +
                 static_cast<int>(Outcome::kLose);
        break;
    }
  }
  return score;
}

int main(int argc, char** argv) {
  if (argc != 2) {
    std::cerr << "usage: aoc_3 STRATEGY" << std::endl;
    exit(EXIT_FAILURE);
  }

  std::cout << "Total Score: " << GetScore(argv[1]) << std::endl;

  exit(EXIT_SUCCESS);
}
