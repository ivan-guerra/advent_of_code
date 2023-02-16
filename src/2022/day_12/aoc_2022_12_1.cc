#include <climits>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using Position = std::pair<int, int>;
struct PositionHash {
    std::size_t operator()(const Position& p) const {
        return std::hash<int>()(p.first) ^ std::hash<int>()(p.second);
    }
};

using TerrainMatrix = std::vector<std::vector<char>>;
using TerrainGraph =
    std::unordered_map<Position, std::vector<Position>, PositionHash>;

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

TerrainGraph InitTerrainGraph(const TerrainMatrix& terrain) {
    TerrainGraph terrain_graph;

    enum class Direction { kUp, kDown, kLeft, kRight };
    std::vector<Direction> directions = {Direction::kUp, Direction::kDown,
                                         Direction::kLeft, Direction::kRight};

    for (int i = 0; i < terrain.size(); i++) {
        for (int j = 0; j < terrain[0].size(); ++j) {
            Position pos(i, j);
            for (const Direction& dir : directions) {
                int rr = i;
                int cc = j;
                switch (dir) {
                    case Direction::kUp:
                        rr += -1;
                        break;
                    case Direction::kDown:
                        rr += 1;
                        break;
                    case Direction::kLeft:
                        cc += -1;
                        break;
                    case Direction::kRight:
                        cc += 1;
                        break;
                }
                if ((rr >= 0) && (rr < terrain.size()) && (cc >= 0) &&
                    (cc <= terrain[0].size()) &&
                    (static_cast<int>(terrain[rr][cc]) -
                         static_cast<int>(terrain[i][j]) <=
                     1)) {
                    terrain_graph[pos].push_back(Position(rr, cc));
                }
            }
        }
    }
    return terrain_graph;
}

std::pair<Position, Position> GetStartAndEndPositions(
    const TerrainMatrix& terrain) {
    std::pair<Position, Position> ret;
    for (int i = 0; i < terrain.size(); i++) {
        for (int j = 0; j < terrain[0].size(); ++j) {
            if (terrain[i][j] == 'S') ret.first = Position(i, j);
            if (terrain[i][j] == 'E') ret.second = Position(i, j);
        }
    }
    return ret;
}

Position MinDistance(
    const std::vector<std::vector<int>>& dp,
    const std::unordered_set<Position, PositionHash>& visited) {
    Position min_pos(-1, -1);
    int min = INT_MAX;
    for (int i = 0; i < dp.size(); ++i) {
        for (int j = 0; j < dp[0].size(); ++j) {
            Position pos(i, j);
            if (!visited.count(pos) && dp[i][j] < min) {
                min_pos = pos;
                min = dp[i][j];
            }
        }
    }
    return min_pos;
}

int FindMinPathLength(TerrainMatrix& terrain) {
    std::pair<Position, Position> start_and_end =
        GetStartAndEndPositions(terrain);
    terrain[start_and_end.first.first][start_and_end.first.second] = 'a';
    terrain[start_and_end.second.first][start_and_end.second.second] = 'z';

    // Create a graph using the 2D terrain matrix.
    TerrainGraph terrain_graph = InitTerrainGraph(terrain);

    // Distance matrix used to track min distance of any vertex from src.
    const int kInfinity = INT_MAX;
    std::vector<std::vector<int>> dp(
        terrain.size(), std::vector<int>(terrain[0].size(), kInfinity));

    // Set the distance to the source to 0.
    dp[start_and_end.first.first][start_and_end.first.second] = 0;

    // Track visited nodes.
    std::unordered_set<Position, PositionHash> visited;

    // Execute Dijkstra's shortest path algorithm.
    Position min_pos = MinDistance(dp, visited);
    while ((min_pos.first != -1) && (min_pos.second != -1)) {
        int cost = dp[min_pos.first][min_pos.second];
        for (const Position& neighbor : terrain_graph[min_pos]) {
            int new_cost = cost + 1;
            if (dp[neighbor.first][neighbor.second] > new_cost) {
                dp[neighbor.first][neighbor.second] = new_cost;
            }
        }
        visited.insert(min_pos);
        min_pos = MinDistance(dp, visited);
    }
    return dp[start_and_end.second.first][start_and_end.second.second];
}

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "usage: aoc INPUT_FILE" << std::endl;
        exit(EXIT_FAILURE);
    }
    TerrainMatrix terrain = LoadTerrain(argv[1]);
    std::cout << "Min Cost: " << FindMinPathLength(terrain) << std::endl;

    exit(EXIT_SUCCESS);
}
