#include <unistd.h>

#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

struct Cart;
using Track = std::vector<std::string>;
using CartList = std::vector<Cart>;

enum class IntersectMove {
  kLeft,
  kRight,
  kStraight,
};

enum Direction {
  kNorth,
  kSouth,
  kEast,
  kWest,
};

struct Cart {
  int x;
  int y;
  Direction dir;
  IntersectMove intersect_move;
  char curr_path_comp;
};

struct {
  bool operator()(const Cart& left, const Cart& right) const {
    return (left.x < right.x) || ((left.x == right.x) && (left.y < right.y));
  }
} CartLess;

Track LoadTrack(const std::string& infile) {
  Track track;
  std::ifstream input(infile);
  std::string line;
  while (std::getline(input, line)) {
    track.push_back(line);
  }
  return track;
}

CartList LoadCarts(const Track& track) {
  CartList carts;
  const std::unordered_map<char, std::pair<Direction, char>> kCartConfigs = {
      {'>', {Direction::kEast, '-'}},
      {'<', {Direction::kWest, '-'}},
      {'^', {Direction::kNorth, '|'}},
      {'v', {Direction::kSouth, '|'}},
  };
  for (int i = 0; i < track.size(); ++i) {
    for (int j = 0; j < track[i].size(); ++j) {
      if (kCartConfigs.count(track[i][j])) {
        Cart cart = {
            .x = i,
            .y = j,
            .dir = kCartConfigs.at(track[i][j]).first,
            .intersect_move = IntersectMove::kLeft,
            .curr_path_comp = kCartConfigs.at(track[i][j]).second,
        };
        carts.push_back(cart);
      }
    }
  }
  return carts;
}

int FindCartIntersection(const CartList& carts) {
  /* The following code assumes the carts are sorted by their x/y coordinate!
   */
  for (int i = 1; i < carts.size(); ++i) {
    if ((carts[i - 1].x == carts[i].x) && (carts[i - 1].y == carts[i].y)) {
      return i;
    }
  }
  return -1; /* no carts intersect */
}

void RedirectCart(Cart& cart) {
  switch (cart.curr_path_comp) {
    case '+':
      if (cart.intersect_move == IntersectMove::kLeft) {
        switch (cart.dir) {
          case Direction::kNorth:
            cart.dir = Direction::kWest;
            break;
          case Direction::kSouth:
            cart.dir = Direction::kEast;
            break;
          case Direction::kWest:
            cart.dir = Direction::kSouth;
            break;
          case Direction::kEast:
            cart.dir = Direction::kNorth;
            break;
        }
        cart.intersect_move = IntersectMove::kStraight;
      } else if (cart.intersect_move == IntersectMove::kStraight) {
        cart.intersect_move = IntersectMove::kRight;
      } else if (cart.intersect_move == IntersectMove::kRight) {
        switch (cart.dir) {
          case Direction::kNorth:
            cart.dir = Direction::kEast;
            break;
          case Direction::kSouth:
            cart.dir = Direction::kWest;
            break;
          case Direction::kWest:
            cart.dir = Direction::kNorth;
            break;
          case Direction::kEast:
            cart.dir = Direction::kSouth;
            break;
        }
        cart.intersect_move = IntersectMove::kLeft;
      }
      break;
    case '\\':
      if (cart.dir == Direction::kWest) {
        cart.dir = Direction::kNorth;
      } else if (cart.dir == Direction::kEast) {
        cart.dir = Direction::kSouth;
      } else if (cart.dir == Direction::kNorth) {
        cart.dir = Direction::kWest;
      } else if (cart.dir == Direction::kSouth) {
        cart.dir = Direction::kEast;
      }
      break;
    case '/':
      if (cart.dir == Direction::kWest) {
        cart.dir = Direction::kSouth;
      } else if (cart.dir == Direction::kEast) {
        cart.dir = Direction::kNorth;
      } else if (cart.dir == Direction::kNorth) {
        cart.dir = Direction::kEast;
      } else if (cart.dir == Direction::kSouth) {
        cart.dir = Direction::kWest;
      }
      break;
    case '|':
    case '-':
      break;
  }
}

void MoveCart(Track& track, Cart& cart) {
  using PosDelta = std::pair<int, int>;
  static const std::unordered_map<Direction, PosDelta> kDirDelta = {
      {Direction::kNorth, {-1, 0}},
      {Direction::kSouth, {1, 0}},
      {Direction::kEast, {0, 1}},
      {Direction::kWest, {0, -1}},
  };

  /* Put down the track component as the cart moves from its current
   * location. */
  track[cart.x][cart.y] = cart.curr_path_comp;

  /* Move the cart according to its current orientation. */
  cart.x += kDirDelta.at(cart.dir).first;
  cart.y += kDirDelta.at(cart.dir).second;

  /* Save off the track component at the cart's new location. */
  cart.curr_path_comp = track[cart.x][cart.y];

  /* Update cart orientation. */
  RedirectCart(cart);
  /* Set the cart marker according to it's new orientation. */
  static const std::unordered_map<Direction, char> kCartIconByDirection = {
      {Direction::kNorth, '^'},
      {Direction::kSouth, 'v'},
      {Direction::kEast, '>'},
      {Direction::kWest, '<'},
  };
  track[cart.x][cart.y] = kCartIconByDirection.at(cart.dir);
}

int RunSimulation(Track& track, CartList& carts) {
  int crashed_cart = -1;
  while ((crashed_cart = FindCartIntersection(carts)) < 0) {
    for (Cart& cart : carts) {
      MoveCart(track, cart);
    }
    std::sort(carts.begin(), carts.end(), CartLess);
  }
  return crashed_cart;
}

int main(int argc, char** argv) {
  if (argc != 2) {
    std::cerr << "error: missing puzzle input file" << std::endl;
    exit(EXIT_FAILURE);
  }

  Track track = LoadTrack(argv[1]);
  CartList carts = LoadCarts(track);
  std::sort(carts.begin(), carts.end(), CartLess);

  int crashed_cart = RunSimulation(track, carts);
  std::cout << "Answer: " << carts[crashed_cart].y << ", "
            << carts[crashed_cart].x << std::endl;

  exit(EXIT_SUCCESS);
}
