#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>

enum Hand { kRock = 1, kPaper = 2, kScissor = 3 };
enum Outcome { kWin = 6, kDraw = 3, kLose = 0 };

Outcome Play(const Hand& opponent, const Hand& player) {
  static const std::unordered_map<Hand, Hand> kWinningMoves = {
      {Hand::kRock, Hand::kScissor},
      {Hand::kPaper, Hand::kRock},
      {Hand::kScissor, Hand::kPaper}};

  if (opponent == player)
    return Outcome::kDraw;
  else if (kWinningMoves.at(player) == opponent)
    return Outcome::kWin;
  else
    return Outcome::kLose;
}

int GetScore(const std::string input) {
  std::ifstream strategy(input);

  int score = 0;
  std::string line;
  static const std::unordered_map<char, Hand> kOpponentHand = {
      {'A', Hand::kRock}, {'B', Hand::kPaper}, {'C', Hand::kScissor}};
  static const std::unordered_map<char, Hand> kPlayerHand = {
      {'X', Hand::kRock}, {'Y', Hand::kPaper}, {'Z', Hand::kScissor}};
  while (std::getline(strategy, line)) {
    score += static_cast<int>(
                 Play(kOpponentHand.at(line[0]), kPlayerHand.at(line[2]))) +
             static_cast<int>(kPlayerHand.at(line[2]));
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
