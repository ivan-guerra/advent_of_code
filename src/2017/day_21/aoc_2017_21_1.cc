#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <fstream>
#include <functional>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

using Matrix = std::vector<std::string>;
using RuleMap = std::unordered_map<std::string, std::string>;

void ParseRule(const std::string rule, RuleMap& rules) {
    std::istringstream iss(rule);
    std::vector<std::string> tokens;
    std::copy(std::istream_iterator<std::string>(iss),
              std::istream_iterator<std::string>(), std::back_inserter(tokens));

    rules[tokens[0]] = tokens[2];
}

Matrix FlipHorizontally(const Matrix& mat) {
    Matrix ret = mat;
    for (std::string& row : ret) {
        std::reverse(row.begin(), row.end());
    }
    return ret;
}

Matrix FlipVertically(const Matrix& mat) {
    Matrix ret = mat;
    for (int i = 0; i < mat.size(); ++i) {
        for (int j = 0; j < mat.size(); ++j) {
            ret[i][j] = mat[mat.size() - i - 1][j];
        }
    }
    return ret;
}

Matrix FlipAntiDiagonally(const Matrix& mat) {
    Matrix ret = mat;
    for (int i = 0; i < mat.size(); ++i) {
        for (int j = 0; j < mat.size(); ++j) {
            ret[i][j] = mat[mat.size() - j - 1][mat.size() - i - 1];
        }
    }
    return ret;
}

Matrix FlipDiagonally(const Matrix& mat) {
    Matrix ret = mat;
    for (int i = 0; i < mat.size(); ++i) {
        for (int j = 0; j < mat.size(); ++j) {
            ret[i][j] = mat[j][i];
        }
    }
    return ret;
}

void RotateMatrix(Matrix& mat) {
    const int kDim = mat.size();
    for (int i = 0; i < (kDim / 2); ++i) {
        for (int j = i; j < (kDim - i - 1); ++j) {
            int temp = mat[i][j];
            mat[i][j] = mat[j][kDim - 1 - i];
            mat[j][kDim - 1 - i] = mat[kDim - 1 - i][kDim - 1 - j];
            mat[kDim - 1 - i][kDim - 1 - j] = mat[kDim - 1 - j][i];
            mat[kDim - 1 - j][i] = temp;
        }
    }
}

Matrix StrToMatrix(const std::string s) {
    Matrix mat;
    std::string row;
    for (const char& c : s) {
        if (c == '/') {
            mat.push_back(row);
            row = "";
        } else {
            row += c;
        }
    }
    mat.push_back(row);

    return mat;
}

std::string MatrixToStr(const Matrix& mat) {
    std::string ret;
    for (const std::string& row : mat) {
        ret += row + "/";
    }
    ret.pop_back();

    return ret;
}

std::vector<Matrix> DivideMatrix(const Matrix& mat) {
    std::vector<Matrix> ret;
    int divisor = (0 == (mat.size() % 2)) ? 2 : 3;
    for (int i = 0; i < mat.size(); i += divisor) {
        for (int j = 0; j < mat[0].size(); j += divisor) {
            Matrix quotient;
            for (int row = i; row < (i + divisor); ++row) {
                std::string curr_row;
                for (int col = j; col < (j + divisor); ++col) {
                    curr_row += mat[row][col];
                }
                quotient.push_back(curr_row);
            }
            ret.push_back(quotient);
        }
    }
    return ret;
}

Matrix MergeMatrices(const std::vector<Matrix>& matrices, int size) {
    Matrix merge(size);

    int chunks = size / matrices[0].size();
    int row = 0;
    for (int k = 0; k < matrices.size(); k += chunks) {
        for (int i = 0; i < matrices[k].size(); ++i) {
            for (int j = k; j < (k + chunks); ++j) {
                merge[row] += matrices[j][i];
            }
            row++;
        }
    }
    return merge;
}

Matrix MatchRule(Matrix& mat, const RuleMap& rules) {
    /* Check for a match at all possible rotations. */
    std::string candidate;
    for (int i = 0; i < 4; ++i) {
        RotateMatrix(mat);
        candidate = MatrixToStr(mat);
        if (rules.count(candidate)) {
            return StrToMatrix(rules.at(candidate));
        }
    }

    static const std::vector<std::function<Matrix(const Matrix&)>>
        kFlipFunctions = {FlipHorizontally, FlipVertically, FlipDiagonally,
                          FlipAntiDiagonally};

    /* Check if flipping the matrix gives a match. */
    for (auto& flip_func : kFlipFunctions) {
        candidate = MatrixToStr(flip_func(mat));
        if (rules.count(candidate)) {
            return StrToMatrix(rules.at(candidate));
        }
    }

    return Matrix(); /* unreachable */
}

Matrix TransformMatrix(Matrix init, const RuleMap& rules, int num_iter) {
    Matrix curr = init;
    while (num_iter--) {
        std::vector<Matrix> submatrices = DivideMatrix(curr);
        std::vector<Matrix> new_submatrices;
        for (Matrix& mat : submatrices) {
            new_submatrices.push_back(MatchRule(mat, rules));
        }
        if (new_submatrices.size() > 1) {
            int new_size = std::sqrt(
                (new_submatrices[0].size() * new_submatrices[0].size()) *
                new_submatrices.size());
            curr = MergeMatrices(new_submatrices, new_size);
        } else {
            curr = new_submatrices[0];
        }
    }
    return curr;
}

int CountOnPixels(const Matrix& mat) {
    int pixel_ctr = 0;
    for (const std::string& row : mat) {
        for (const char& pixel : row) {
            if (pixel == '#') {
                pixel_ctr++;
            }
        }
    }
    return pixel_ctr;
}

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "error: missing puzzle input file" << std::endl;
        exit(EXIT_FAILURE);
    }

    std::ifstream input(argv[1]);
    std::string rule;
    RuleMap rules;
    while (std::getline(input, rule)) {
        ParseRule(rule, rules);
    }

    const int kNumIter = 5;
    Matrix init = StrToMatrix(".#./..#/###");
    Matrix out = TransformMatrix(init, rules, kNumIter);

    std::cout << "Answer: " << CountOnPixels(out) << std::endl;

    exit(EXIT_SUCCESS);
}
