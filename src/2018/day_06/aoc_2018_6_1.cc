#include <algorithm>
#include <climits>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <iterator>
#include <queue>
#include <sstream>
#include <string>
#include <unordered_set>
#include <vector>

struct Point {
    int id = -1;
    int x = -1;
    int y = -1;

    friend bool operator==(const Point& a, const Point& b) {
        return ((a.x == b.x) && (a.y == b.y));
    }
};

struct PointHash {
    std::size_t operator()(const Point& p) const {
        std::string hash_str = std::to_string(p.x) + "," + std::to_string(p.y);
        return std::hash<std::string>{}(hash_str);
    }
};

Point ParsePoint(const std::string& text) {
    std::istringstream iss(text);
    std::vector<std::string> words;
    std::copy(std::istream_iterator<std::string>(iss),
              std::istream_iterator<std::string>(), std::back_inserter(words));

    static int point_id = 0;
    Point point;
    point.id = point_id++;
    point.x = std::stoi(words[0].substr(0, words[0].size() - 1));
    point.y = std::stoi(words[1]);

    return point;
}

int Distance(const Point& a, const Point& b) {
    return (std::abs(a.x - b.x) + std::abs(a.y - b.y));
}

bool IsContainedWithinArea(const Point& src, const Point& candidate,
                           const std::vector<Point>& points) {
    int dist_src_to_candidate = Distance(src, candidate);
    for (const Point& p : points) {
        if (p.id != src.id) {
            int dist = Distance(p, candidate);
            if (dist <= dist_src_to_candidate) {
                return false;
            }
        }
    }
    return true;
}

bool HasFiniteArea(const std::vector<Point>& points, const Point& point) {
    using Direction = std::pair<int, int>;
    static const std::vector<Direction> directions = {
        {0, 1}, {1, 0}, {-1, 0}, {0, -1}, {1, 1}, {-1, -1}, {-1, 1}, {1, -1},
    };
    for (const Direction& dir : directions) {
        const int kSearchLimit = 1000000;
        Point candidate = point;
        int i = 0;
        while (i < kSearchLimit) {
            candidate.x += dir.first;
            candidate.y += dir.second;
            if (!IsContainedWithinArea(point, candidate, points)) {
                break;
            }
            i++;
        }

        if (i == kSearchLimit) {
            return false;
        }
    }
    return true;
}

std::vector<Point> GetNeighbors(const Point& src, const Point& curr,
                                const std::vector<Point>& points) {
    using Direction = std::pair<int, int>;
    static const std::vector<Direction> directions = {
        {0, 1}, {1, 0}, {-1, 0}, {0, -1}, {1, 1}, {-1, -1}, {-1, 1}, {1, -1},
    };
    std::vector<Point> neighbors;
    for (const Direction& dir : directions) {
        Point candidate = {
            .id = -1,
            .x = curr.x + dir.first,
            .y = curr.y + dir.second,
        };
        if (IsContainedWithinArea(src, candidate, points)) {
            neighbors.push_back(candidate);
        }
    }
    return neighbors;
}

int ComputeArea(const Point& src, const std::vector<Point>& points) {
    std::unordered_set<Point, PointHash> visited;
    std::queue<Point> nodes;
    nodes.push(src);

    int total_area = 0;
    while (!nodes.empty()) {
        Point curr = nodes.front();
        nodes.pop();
        if (visited.count(curr)) {
            continue;
        }

        total_area++;
        visited.insert(curr);
        auto neighbors = GetNeighbors(src, curr, points);
        for (const Point& p : neighbors) {
            if (!visited.count(p)) {
                nodes.push(p);
            }
        }
    }
    return total_area;
}

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "error: missing puzzle input file" << std::endl;
        exit(EXIT_FAILURE);
    }

    std::ifstream input(argv[1]);
    std::string line;
    std::vector<Point> points;
    while (std::getline(input, line)) {
        points.push_back(ParsePoint(line));
    }

    std::vector<int> finite_areas;
    for (const Point& p : points) {
        if (HasFiniteArea(points, p)) {
            std::cout << "finite: " << p.x << ", " << p.y << std::endl;
            finite_areas.push_back(ComputeArea(p, points));
        }
    }

    std::cout << "Answer: "
              << *std::max_element(finite_areas.begin(), finite_areas.end())
              << std::endl;

    exit(EXIT_SUCCESS);
}
