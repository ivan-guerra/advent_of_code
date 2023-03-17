#include <algorithm>
#include <climits>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <queue>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

struct MapCell;
struct MapCellHash;

using HvacMap = std::vector<std::string>;
using MapCellList = std::vector<MapCell>;
using MinDistGraph =
    std::unordered_map<MapCell, std::unordered_map<MapCell, int, MapCellHash>,
                       MapCellHash>;

struct MapCell {
    int x = 0;
    int y = 0;
    int id = 0;

    MapCell(int x_, int y_, int id_) : x(x_), y(y_), id(id_) {}

    friend bool operator==(const MapCell& a, const MapCell& b) {
        return ((a.x == b.x) && (a.y == b.y) && (a.id == b.id));
    }
    friend bool operator<(const MapCell& a, const MapCell& b) {
        return (a.id < b.id);
    }
};

struct MapCellHash {
    std::size_t operator()(const MapCell& cell) const {
        std::string hash_str =
            std::to_string(cell.x) + "," + std::to_string(cell.y);
        return std::hash<std::string>{}(hash_str);
    }
};

MapCellList GetTargets(const HvacMap& map) {
    MapCellList targets;
    for (int i = 0; i < map.size(); ++i) {
        for (int j = 0; j < map[0].size(); ++j) {
            if (std::isdigit(map[i][j])) {
                targets.emplace_back(i, j, std::atoi(&map[i][j]));
            }
        }
    }
    return targets;
}

MapCellList GetNeighbors(const HvacMap& map, const MapCell& node) {
    enum Direction {
        kUp,
        kDown,
        kLeft,
        kRight,
    };
    static const std::vector<Direction> kDirs = {
        Direction::kUp,
        Direction::kDown,
        Direction::kLeft,
        Direction::kRight,
    };

    MapCellList neighbors;
    for (const Direction& dir : kDirs) {
        int row = node.x;
        int col = node.y;
        switch (dir) {
            case Direction::kUp:
                row--;
                break;
            case Direction::kDown:
                row++;
                break;
            case Direction::kLeft:
                col--;
                break;
            case Direction::kRight:
                col++;
                break;
        }
        if ((row >= 0) && (row < map.size()) && (col >= 0) &&
            (col < map[0].size()) && (map[row][col] != '#')) {
            if (map[row][col] == '.') {
                neighbors.emplace_back(row, col, -1);
            } else {
                neighbors.emplace_back(row, col, std::atoi(&map[row][col]));
            }
        }
    }
    return neighbors;
}

int ComputeShortestPath(const HvacMap& map, const MapCell& src,
                        const MapCell& dst) {
    std::queue<MapCell> nodes;
    nodes.push(src);
    std::unordered_set<MapCell, MapCellHash> visited;
    int dist = 0;
    while (!nodes.empty()) {
        int num_nodes = nodes.size();
        for (int i = 0; i < num_nodes; ++i) {
            MapCell curr = nodes.front();
            nodes.pop();

            if (visited.count(curr)) {
                continue;
            }
            if (curr == dst) {
                return dist;
            }

            visited.insert(curr);
            MapCellList neighbors = GetNeighbors(map, curr);
            for (const MapCell& cell : neighbors) {
                if (!visited.count(cell)) {
                    nodes.push(cell);
                }
            }
        }
        dist++;
    }
    return INT_MAX; /* No path exists! */
}

MinDistGraph GetMinDistGraph(const HvacMap& map, const MapCellList& targets) {
    MinDistGraph min_dists;
    for (int i = 0; i < targets.size(); ++i) {
        const MapCell& src = targets[i];
        for (int j = 0; j < targets.size(); ++j) {
            const MapCell& dst = targets[j];
            if (min_dists.count(dst) && min_dists.at(dst).count(src)) {
                min_dists[src][dst] = min_dists[dst][src];
            } else {
                min_dists[src][dst] = ComputeShortestPath(map, src, dst);
            }
        }
    }
    return min_dists;
}

int GetCostOfShortestTour(const MinDistGraph& min_dists, MapCellList& targets) {
    std::sort(targets.begin(), targets.end());
    MapCell start = targets[0];

    int min_tour_cost = INT_MAX;
    MapCellList rest(targets.begin() + 1, targets.end());
    do {
        int tour_cost = min_dists.at(start).at(rest[0]);
        for (int i = 1; i < rest.size(); ++i) {
            tour_cost += min_dists.at(rest[i - 1]).at(rest[i]);
        }
        tour_cost += min_dists.at(rest.back()).at(start);

        min_tour_cost = std::min(min_tour_cost, tour_cost);
    } while (std::next_permutation(rest.begin(), rest.end()));
    return min_tour_cost;
}

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "error: missing puzzle input file" << std::endl;
        exit(EXIT_FAILURE);
    }

    std::ifstream input(argv[1]);
    std::string row;
    HvacMap map;
    while (std::getline(input, row)) {
        map.push_back(row);
    }

    MapCellList targets = GetTargets(map);
    MinDistGraph min_dists = GetMinDistGraph(map, targets);

    std::cout << "Answer: " << GetCostOfShortestTour(min_dists, targets)
              << std::endl;

    exit(EXIT_SUCCESS);
}
