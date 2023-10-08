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

bool IsVisible(int i, int j, Dir direction, const TreeMatrix& tree_mat) {
  int r = tree_mat.size();
  int c = tree_mat[0].size();
  int curr_tree = tree_mat[i][j];
  switch (direction) {
    case Dir::kRight:
      for (int k = j + 1; k < c; ++k) {
        if (curr_tree <= tree_mat[i][k]) return false;
      }
      break;
    case Dir::kLeft:
      for (int k = j - 1; k >= 0; --k) {
        if (curr_tree <= tree_mat[i][k]) return false;
      }
      break;
    case Dir::kUp:
      for (int k = i - 1; k >= 0; --k) {
        if (curr_tree <= tree_mat[k][j]) return false;
      }
      break;
    case Dir::kDown:
      for (int k = i + 1; k < r; ++k) {
        if (curr_tree <= tree_mat[k][j]) return false;
      }
      break;
  }
  return true;
}

int GetNumVisibleTrees(const TreeMatrix& tree_mat) {
  int visible = 0;
  for (int i = 0; i < tree_mat.size(); ++i) {
    for (int j = 0; j < tree_mat[0].size(); ++j) {
      if (IsOnEdge(i, j, tree_mat) || IsVisible(i, j, Dir::kUp, tree_mat) ||
          IsVisible(i, j, Dir::kDown, tree_mat) ||
          IsVisible(i, j, Dir::kLeft, tree_mat) ||
          IsVisible(i, j, Dir::kRight, tree_mat)) {
        visible++;
      }
    }
  }
  return visible;
}

int main(int argc, char** argv) {
  if (argc != 2) {
    std::cerr << "usage: aoc INPUT_FILE" << std::endl;
    exit(EXIT_FAILURE);
  }

  TreeMatrix tm = LoadTreeData(argv[1]);
  std::cout << "Number of Visible Trees: " << GetNumVisibleTrees(tm)
            << std::endl;

  exit(EXIT_SUCCESS);
}
