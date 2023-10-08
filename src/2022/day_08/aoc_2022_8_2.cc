#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using TreeMatrix = std::vector<std::vector<int>>;

enum Dir { kUp, kDown, kRight, kLeft };

TreeMatrix LoadTreeData(const std::string& input) {
  std::ifstream tree_if(input);

  int i = 0;
  std::string line;
  TreeMatrix tree_mat;
  while (std::getline(tree_if, line)) {
    tree_mat.emplace_back();
    for (const char& c : line) {
      tree_mat[i].push_back(c - '0');
    }
    i++;
  }
  return tree_mat;
}

bool IsOnEdge(int i, int j, const TreeMatrix& tree_mat) {
  return ((0 == i) || (0 == j) || (i == tree_mat.size() - 1) ||
          (j == tree_mat[0].size() - 1));
}

int GetScenicScore(int i, int j, const TreeMatrix& tree_mat) {
  int r = tree_mat.size();
  int c = tree_mat[0].size();
  int curr_tree = tree_mat[i][j];

  int scenic_score = 1;
  std::vector<Dir> directions = {Dir::kUp, Dir::kDown, Dir::kLeft, Dir::kRight};
  for (const Dir& dir : directions) {
    int score = 0;
    switch (dir) {
      case Dir::kRight:
        for (int k = j + 1; k < c; ++k) {
          score++;
          if (IsOnEdge(i, k, tree_mat) || curr_tree <= tree_mat[i][k]) break;
        }
        break;
      case Dir::kLeft:
        for (int k = j - 1; k >= 0; --k) {
          score++;
          if (IsOnEdge(i, k, tree_mat) || curr_tree <= tree_mat[i][k]) break;
        }
        break;
      case Dir::kUp:
        for (int k = i - 1; k >= 0; --k) {
          score++;
          if (IsOnEdge(i, k, tree_mat) || curr_tree <= tree_mat[k][j]) break;
        }
        break;
      case Dir::kDown:
        for (int k = i + 1; k < r; ++k) {
          score++;
          if (IsOnEdge(i, k, tree_mat) || curr_tree <= tree_mat[k][j]) break;
        }
        break;
    }
    scenic_score *= score;
  }
  return scenic_score;
}

int GetMaxScenicScore(const TreeMatrix& tree_mat) {
  int max_score = 0;
  for (int i = 0; i < tree_mat.size(); ++i) {
    for (int j = 0; j < tree_mat[0].size(); ++j) {
      if (!IsOnEdge(i, j, tree_mat))
        max_score = std::max(max_score, GetScenicScore(i, j, tree_mat));
    }
  }
  return max_score;
}

int main(int argc, char** argv) {
  if (argc != 2) {
    std::cerr << "usage: aoc INPUT_FILE" << std::endl;
    exit(EXIT_FAILURE);
  }

  TreeMatrix tm = LoadTreeData(argv[1]);
  std::cout << "Max Scenic Score: " << GetMaxScenicScore(tm) << std::endl;

  exit(EXIT_SUCCESS);
}
