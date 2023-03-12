#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

enum class TileType {
    kTrap,
    kSafe,
};

using TrapMatrix = std::vector<std::vector<TileType>>;

bool IsTrap(const std::vector<TileType>& tiles) {
    int num_traps = 0;
    for (const TileType& tile : tiles) {
        if (tile == TileType::kTrap) {
            num_traps++;
        }
    }

    TileType left = tiles[0];
    TileType center = tiles[1];
    TileType right = tiles[2];
    if (num_traps == 3) {
        return false;
    } else if (num_traps == 2) {
        return (center == TileType::kTrap);
    } else if (num_traps == 1) {
        return ((left == TileType::kTrap) || (right == TileType::kTrap));
    }
    return false;
}

TileType GetTileType(const TrapMatrix& mat, int i, int j) {
    enum TilePosition {
        kLeft,
        kCenter,
        kRight,
    };
    static const std::vector<TilePosition> kPositions = {
        TilePosition::kLeft,
        TilePosition::kCenter,
        TilePosition::kRight,
    };

    std::vector<TileType> neighbor_tiles;
    for (const TilePosition& pos : kPositions) {
        int row = i;
        int col = j;
        switch (pos) {
            case TilePosition::kLeft:
                row--;
                col--;
                break;
            case TilePosition::kCenter:
                row--;
                break;
            case TilePosition::kRight:
                row--;
                col++;
                break;
        }
        if ((row >= 0) && (row < mat.size()) && (col >= 0) &&
            (col < mat[0].size())) {
            if (mat[row][col] == TileType::kTrap) {
                neighbor_tiles.push_back(TileType::kTrap);
            } else {
                neighbor_tiles.push_back(TileType::kSafe);
            }
        } else {
            neighbor_tiles.push_back(TileType::kSafe);
        }
    }
    return (IsTrap(neighbor_tiles)) ? TileType::kTrap : TileType::kSafe;
}

int GetNumSafeTiles(const std::string& init_row, int num_rows) {
    int num_safe_tiles = 0;
    TrapMatrix mat(num_rows,
                   std::vector<TileType>(init_row.size(), TileType::kSafe));
    for (int i = 0; i < init_row.size(); ++i) {
        if ('.' == init_row[i]) {
            num_safe_tiles++;
            mat[0][i] = TileType::kSafe;
        } else {
            mat[0][i] = TileType::kTrap;
        }
    }

    for (int i = 1; i < num_rows; ++i) {
        for (int j = 0; j < init_row.size(); ++j) {
            mat[i][j] = GetTileType(mat, i, j);
            if (TileType::kSafe == mat[i][j]) {
                num_safe_tiles++;
            }
        }
    }
    return num_safe_tiles;
}

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "error: missing puzzle input file" << std::endl;
        exit(EXIT_FAILURE);
    }

    std::ifstream input(argv[1]);
    std::string init_row;
    std::getline(input, init_row);

    const int kNumRows = 40;
    std::cout << "Answer: " << GetNumSafeTiles(init_row, kNumRows) << std::endl;

    exit(EXIT_SUCCESS);
}
