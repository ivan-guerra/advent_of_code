#include <climits>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_set>
#include <vector>

struct Point2D {
    int x = 0;
    int y = 0;

    friend bool operator==(const Point2D a, const Point2D b) {
        return ((a.x == b.x) && (a.y == b.y));
    }
};

struct Point2DHash {
    std::size_t operator()(const Point2D& p) const {
        std::string point_str = std::to_string(p.x) + "," + std::to_string(p.y);
        return std::hash<std::string>{}(point_str);
    }
};

enum Direction {
    kUp = 'U',
    kDown = 'D',
    kLeft = 'L',
    kRight = 'R',
};

struct Instruction {
    Direction direction;
    unsigned int magnitude;

    Instruction(const Direction& dir, unsigned int mag)
        : direction(dir), magnitude(mag) {}
};

using PointSet = std::unordered_set<Point2D, Point2DHash>;

std::vector<Instruction> ParseInstructions(const std::string& wire_str) {
    std::vector<Instruction> instructions;
    std::stringstream ss(wire_str);
    std::string token;
    while (std::getline(ss, token, ',')) {
        std::string dir = token.substr(0, 1);
        unsigned int mag = std::stoi(token.substr(1));
        switch (dir[0]) {
            case Direction::kUp:
                instructions.emplace_back(Direction::kUp, mag);
                break;
            case Direction::kDown:
                instructions.emplace_back(Direction::kDown, mag);
                break;
            case Direction::kLeft:
                instructions.emplace_back(Direction::kLeft, mag);
                break;
            case Direction::kRight:
                instructions.emplace_back(Direction::kRight, mag);
                break;
        }
    }
    return instructions;
}

PointSet ParsePoints(const std::vector<Instruction>& instructions) {
    PointSet points;
    Point2D curr_loc = {0, 0};
    for (const Instruction& instr : instructions) {
        for (int i = 0; i < instr.magnitude; ++i) {
            switch (instr.direction) {
                case Direction::kUp:
                    curr_loc.y -= 1;
                    points.insert(curr_loc);
                    break;
                case Direction::kDown:
                    curr_loc.y += 1;
                    points.insert(curr_loc);
                    break;
                case Direction::kLeft:
                    curr_loc.x -= 1;
                    points.insert(curr_loc);
                    break;
                case Direction::kRight:
                    curr_loc.x += 1;
                    points.insert(curr_loc);
                    break;
            }
        }
    }
    return points;
}

PointSet GetWirePoints(const std::string& wire) {
    auto instructions = ParseInstructions(wire);
    return ParsePoints(instructions);
}

PointSet Intersection(const PointSet& wire1, const PointSet& wire2) {
    PointSet intersection;
    for (const Point2D p : wire1) {
        if (wire2.count(p)) {
            intersection.insert(p);
        }
    }
    return intersection;
}

int ComputeMinIntersectDist(const PointSet& wire1, const PointSet& wire2) {
    auto ManhattanDist = [](const Point2D a, const Point2D b) {
        return (std::abs(a.x - b.x) + std::abs(a.y + b.y));
    };

    PointSet intersection = Intersection(wire1, wire2);
    int min_dist = INT_MAX;
    Point2D origin = {0, 0};
    for (const Point2D& p : intersection) {
        min_dist = std::min(min_dist, ManhattanDist(origin, p));
    }
    return min_dist;
}

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "error: missing puzzle input file" << std::endl;
        exit(EXIT_FAILURE);
    }

    std::ifstream input(argv[1]);
    std::string wire1;
    std::string wire2;
    std::getline(input, wire1);
    std::getline(input, wire2);

    PointSet wire1_points = GetWirePoints(wire1);
    PointSet wire2_points = GetWirePoints(wire2);

    std::cout << "Answer: "
              << ComputeMinIntersectDist(wire1_points, wire2_points)
              << std::endl;

    exit(EXIT_SUCCESS);
}
