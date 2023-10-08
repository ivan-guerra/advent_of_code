#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

enum class Direction {
  kUp,
  kDown,
  kLeft,
  kRight,
};

enum class NodeState {
  kCleaned,
  kWeakened,
  kInfected,
  kFlagged,
};

struct Coordinate2D {
  int x = 0;
  int y = 0;

  Coordinate2D(int x_, int y_) : x(x_), y(y_) {}
  friend bool operator==(const Coordinate2D& a, const Coordinate2D& b) {
    return ((a.x == b.x) && (a.y == b.y));
  }
};

struct Coordinate2DHash {
  std::size_t operator()(const Coordinate2D& c) const {
    std::string coord_str = std::to_string(c.x) + "," + std::to_string(c.y);
    return std::hash<std::string>{}(coord_str);
  }
};

using Matrix = std::vector<std::string>;
using TamperedNodeMap =
    std::unordered_map<Coordinate2D, NodeState, Coordinate2DHash>;

void InitTamperedNodeMap(TamperedNodeMap& tampered_nodes,
                         const Matrix& init_map) {
  for (int i = 0; i < init_map.size(); ++i) {
    for (int j = 0; j < init_map[0].size(); ++j) {
      if (init_map[i][j] == '#') {
        Coordinate2D pos(i, j);
        tampered_nodes[pos] = NodeState::kInfected;
      }
    }
  }
}

void TurnRight(Direction& dir) {
  switch (dir) {
    case Direction::kUp:
      dir = Direction::kRight;
      break;
    case Direction::kDown:
      dir = Direction::kLeft;
      break;
    case Direction::kLeft:
      dir = Direction::kUp;
      break;
    case Direction::kRight:
      dir = Direction::kDown;
      break;
  }
}

void TurnLeft(Direction& dir) {
  switch (dir) {
    case Direction::kUp:
      dir = Direction::kLeft;
      break;
    case Direction::kDown:
      dir = Direction::kRight;
      break;
    case Direction::kLeft:
      dir = Direction::kDown;
      break;
    case Direction::kRight:
      dir = Direction::kUp;
      break;
  }
}

void ReverseDirection(Direction& dir) {
  switch (dir) {
    case Direction::kUp:
      dir = Direction::kDown;
      break;
    case Direction::kDown:
      dir = Direction::kUp;
      break;
    case Direction::kLeft:
      dir = Direction::kRight;
      break;
    case Direction::kRight:
      dir = Direction::kLeft;
      break;
  }
}

NodeState TransitionNodeState(const NodeState state) {
  switch (state) {
    case NodeState::kCleaned:
      return NodeState::kWeakened;
    case NodeState::kWeakened:
      return NodeState::kInfected;
    case NodeState::kInfected:
      return NodeState::kFlagged;
    case NodeState::kFlagged:
      return NodeState::kCleaned;
  }
  return NodeState::kCleaned;
}

int ComputeNewInfections(TamperedNodeMap& tampered_nodes,
                         const Coordinate2D& origin, int num_bursts) {
  int num_new_infections = 0;
  Direction curr_direction = Direction::kUp;
  Coordinate2D curr_node = origin;
  tampered_nodes[curr_node] = NodeState::kCleaned;
  while (num_bursts--) {
    switch (tampered_nodes[curr_node]) {
      case NodeState::kCleaned:
        TurnLeft(curr_direction);
        break;
      case NodeState::kWeakened:
        break;
      case NodeState::kInfected:
        TurnRight(curr_direction);
        break;
      case NodeState::kFlagged:
        ReverseDirection(curr_direction);
        break;
    }

    NodeState new_state = TransitionNodeState(tampered_nodes[curr_node]);
    tampered_nodes[curr_node] = new_state;
    if (new_state == NodeState::kInfected) {
      num_new_infections++;
    }

    switch (curr_direction) {
      case Direction::kUp:
        curr_node.x--;
        break;
      case Direction::kDown:
        curr_node.x++;
        break;
      case Direction::kLeft:
        curr_node.y--;
        break;
      case Direction::kRight:
        curr_node.y++;
        break;
    }
    if (!tampered_nodes.count(curr_node)) {
      tampered_nodes[curr_node] = NodeState::kCleaned;
    }
  }
  return num_new_infections;
}

int main(int argc, char** argv) {
  if (argc != 2) {
    std::cerr << "error: missing puzzle input file" << std::endl;
    exit(EXIT_FAILURE);
  }

  std::ifstream input(argv[1]);
  std::string line;
  Matrix init_map;
  while (std::getline(input, line)) {
    init_map.push_back(line);
  }

  Coordinate2D origin(init_map.size() / 2, init_map[0].size() / 2);
  TamperedNodeMap tampered_nodes;
  InitTamperedNodeMap(tampered_nodes, init_map);

  const int kNumBursts = 10000000;
  std::cout << "Answer: "
            << ComputeNewInfections(tampered_nodes, origin, kNumBursts)
            << std::endl;

  exit(EXIT_SUCCESS);
}
