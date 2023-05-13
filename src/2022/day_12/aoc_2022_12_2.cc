#include <climits>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <queue>
#include <string>
#include <vector>

using TerrainMatrix = std::vector<std::vector<char>>;
using Position = std::pair<int, int>;

TerrainMatrix LoadTerrain(const std::string& input) {
    std::ifstream terrain_is(input);

    std::string line;
    TerrainMatrix terrain;
    while (std::getline(terrain_is, line)) {
        std::vector<char> row(line.size());
        for (int i = 0; i < line.size(); ++i) row[i] = line[i];
        terrain.push_back(row);
    }
    return terrain;
}

bool IsValidMove(const TerrainMatrix& terrain, char curr, int i, int j) {
    return ((i >= 0) && (i < terrain.size()) && (j >= 0) &&
            (j < terrain[0].size()) && (terrain[i][j] != '.') &&
            ((static_cast<int>(terrain[i][j]) - static_cast<int>(curr)) <= 1));
}

std::vector<Position> GetValidMoves(const TerrainMatrix& terrain, int i,
                                    int j) {
    char curr = terrain[i][j];
    std::vector<Position> valid_moves;

    // Check down.
    if (IsValidMove(terrain, curr, i + 1, j))
        valid_moves.push_back(Position(i + 1, j));

    // Check up.
    if (IsValidMove(terrain, curr, i - 1, j))
        valid_moves.push_back(Position(i - 1, j));

    // Check right.
    if (IsValidMove(terrain, curr, i, j + 1))
        valid_moves.push_back(Position(i, j + 1));

    // Check left.
    if (IsValidMove(terrain, curr, i, j - 1))
        valid_moves.push_back(Position(i, j - 1));

    return valid_moves;
}

int FindMinPathLength(TerrainMatrix terrain, const Position& src,
                      const Position& dst) {
    terrain[src.first][src.second] = 'a';
    terrain[dst.first][dst.second] = 'z';

    std::queue<Position> nodes;
    nodes.push(src);

    // Perform a BFS to find the shortest path from src -> dst.
    int level = 0;
    while (!nodes.empty()) {
        int num_nodes = nodes.size();
        for (int i = 0; i < num_nodes; ++i) {
            Position curr = nodes.front();
            nodes.pop();

            if ((curr.first == dst.first) && (curr.second == dst.second))
                return level;

            auto valid_moves = GetValidMoves(terrain, curr.first, curr.second);
            terrain[curr.first][curr.second] = '.';
            for (const Position& pos : valid_moves) {
                nodes.push(pos);
            }
        }
        level++;
    }
    return INT_MAX;  // There was no path from src -> dst.
}

std::vector<Position> GetStartPoints(TerrainMatrix& terrain) {
    std::vector<Position> start_points;
    for (int i = 0; i < terrain.size(); ++i) {
        for (int j = 0; j < terrain[0].size(); ++j) {
            if ((terrain[i][j] == 'S') || (terrain[i][j] == 'a'))
                start_points.push_back(Position(i, j));
        }
    }
    return start_points;
}

Position GetEndPoint(TerrainMatrix& terrain) {
    for (int i = 0; i < terrain.size(); ++i) {
        for (int j = 0; j < terrain[0].size(); ++j) {
            if (terrain[i][j] == 'E') return Position(i, j);
        }
    }
    return Position(INT_MAX, INT_MAX);  // Unreachable
}

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "usage: aoc INPUT_FILE" << std::endl;
        exit(EXIT_FAILURE);
    }

    TerrainMatrix terrain = LoadTerrain(argv[1]);

    // Find all the possible starting points in the terrain matrix.
    std::vector<Position> start_points = GetStartPoints(terrain);

    // Find the end point in the terrain matrix.
    Position end_point = GetEndPoint(terrain);

    // Find the min path from any start point to the end point.
    int min_cost = INT_MAX;
    for (const Position& sp : start_points) {
        min_cost =
            std::min(min_cost, FindMinPathLength(terrain, sp, end_point));
    }
    std::cout << "Min Cost: " << min_cost << std::endl;

    exit(EXIT_SUCCESS);
}
