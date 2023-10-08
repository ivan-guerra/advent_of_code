#include <cstdlib>
#include <fstream>
#include <iostream>
#include <iterator>
#include <sstream>
#include <unordered_map>
#include <vector>

struct Ingredient {
  std::string name;
  int capacity = 0;
  int durability = 0;
  int flavor = 0;
  int texture = 0;
};

Ingredient ParseIngredient(const std::string& ingredient) {
  std::istringstream iss(ingredient);
  std::vector<std::string> tokens;
  std::copy(std::istream_iterator<std::string>(iss),
            std::istream_iterator<std::string>(), std::back_inserter(tokens));

  auto get_feature_val = [](const std::string& token) {
    return std::stoi(token.substr(0, token.size() - 1));
  };

  Ingredient ret = {
      .name = tokens[0].substr(0, tokens[0].size() - 1),
      .capacity = get_feature_val(tokens[2]),
      .durability = get_feature_val(tokens[4]),
      .flavor = get_feature_val(tokens[6]),
      .texture = get_feature_val(tokens[8]),
  };
  return ret;
}

int main(int argc, char** argv) {
  if (argc != 2) {
    std::cerr << "error: missing puzzle input file" << std::endl;
    exit(EXIT_FAILURE);
  }

  std::ifstream input(argv[1]);
  std::string ingredient;
  std::vector<Ingredient> ingredients;
  while (std::getline(input, ingredient)) {
    ingredients.push_back(ParseIngredient(ingredient));
  }

  const int kNumIngredients = 100;
  int max_score = 0;
  for (int i = 0; i < kNumIngredients; ++i) {
    for (int j = 0; j < (kNumIngredients - i); ++j) {
      for (int k = 0; k < (kNumIngredients - i - j); ++k) {
        int h = kNumIngredients - i - j - k;
        int capacity =
            (ingredients[0].capacity * i) + (ingredients[1].capacity * j) +
            (ingredients[2].capacity * k) + (ingredients[3].capacity * h);
        int durability =
            (ingredients[0].durability * i) + (ingredients[1].durability * j) +
            (ingredients[2].durability * k) + (ingredients[3].durability * h);

        int flavor = (ingredients[0].flavor * i) + (ingredients[1].flavor * j) +
                     (ingredients[2].flavor * k) + (ingredients[3].flavor * h);
        int texture =
            (ingredients[0].texture * i) + (ingredients[1].texture * j) +
            (ingredients[2].texture * k) + (ingredients[3].texture * h);

        if ((capacity <= 0) || (durability <= 0) || (flavor <= 0) ||
            (texture <= 0)) {
          continue;
        }
        max_score =
            std::max(max_score, (capacity * durability * flavor * texture));
      }
    }
  }

  std::cout << "Answer: " << max_score << std::endl;

  exit(EXIT_SUCCESS);
}
