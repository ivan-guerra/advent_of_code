#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <unordered_set>
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
    int id;
    bool active;
    int x;
    int y;
    Direction dir;
    IntersectMove intersect_move;
};

struct {
    bool operator()(const Cart& left, const Cart& right) const {
        return (left.x < right.x) ||
               ((left.x == right.x) && (left.y < right.y));
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

CartList LoadCarts(Track& track) {
    CartList carts;
    static const std::unordered_map<char, std::pair<Direction, char>>
        kCartConfigs = {
            {'>', {Direction::kEast, '-'}},
            {'<', {Direction::kWest, '-'}},
            {'^', {Direction::kNorth, '|'}},
            {'v', {Direction::kSouth, '|'}},
        };

    int id = 0;
    for (int i = 0; i < track.size(); ++i) {
        for (int j = 0; j < track[i].size(); ++j) {
            if (kCartConfigs.count(track[i][j])) {
                Cart cart = {
                    .id = id++,
                    .active = true,
                    .x = i,
                    .y = j,
                    .dir = kCartConfigs.at(track[i][j]).first,
                    .intersect_move = IntersectMove::kLeft,
                };
                carts.push_back(cart);

                /* Erase the cart symbol from the track since we don't really
                 * need it and it complicates updating cart positions. */
                track[i][j] = kCartConfigs.at(track[i][j]).second;
            }
        }
    }
    return carts;
}

std::vector<int> FindIntersectingCarts(const CartList& carts) {
    /* This code assumes the carts are sorted by their x/y coordinate! */
    std::vector<int> crashed;
    for (int i = 1; i < carts.size(); ++i) {
        if (!carts[i - 1].active || !carts[i].active) {
            continue;
        }

        if ((carts[i - 1].x == carts[i].x) && (carts[i - 1].y == carts[i].y)) {
            crashed.push_back(carts[i - 1].id);
            crashed.push_back(carts[i].id);
        }
    }
    return crashed;
}

void RedirectCart(const Track& track, Cart& cart) {
    switch (track[cart.x][cart.y]) {
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
            /* No change in direction. */
            break;
        default:
            std::cout << "Oh my got a unknown char!" << std::endl;
            break;
    }
}

void MoveCart(const Track& track, Cart& cart) {
    using PosDelta = std::pair<int, int>;
    static const std::unordered_map<Direction, PosDelta> kDirDelta = {
        {Direction::kNorth, {-1, 0}},
        {Direction::kSouth, {1, 0}},
        {Direction::kEast, {0, 1}},
        {Direction::kWest, {0, -1}},
    };

    /* Move the cart according to its current orientation. */
    cart.x += kDirDelta.at(cart.dir).first;
    cart.y += kDirDelta.at(cart.dir).second;

    /* Update cart orientation. */
    RedirectCart(track, cart);
}

Cart RunSimulation(Track& track, CartList& carts) {
    auto IsActive = [](const Cart& c) { return c.active; };

    while (std::count_if(carts.begin(), carts.end(), IsActive) > 1) {
        std::vector<int> crashed_cart_ids;
        for (Cart& cart : carts) {
            if (cart.active) {
                MoveCart(track, cart);

                /* Check if the most recent cart move caused a crash. */
                std::vector<int> crash_ids = FindIntersectingCarts(carts);
                crashed_cart_ids.insert(crashed_cart_ids.end(),
                                        crash_ids.begin(), crash_ids.end());
            }
        }

        /* 'Remove' crashed carts by de-activating them. */
        for (const int& id : crashed_cart_ids) {
            auto match =
                std::find_if(carts.begin(), carts.end(),
                             [id](const Cart& c) { return (c.id == id); });
            if (match != carts.end()) {
                match->active = false;
                match->x = match->y = -1;
            }
        }

        /* Sort active carts by coordinate. */
        std::sort(carts.begin(), carts.end(), CartLess);
    }
    return *std::find_if(carts.begin(), carts.end(), IsActive);
}

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "error: missing puzzle input file" << std::endl;
        exit(EXIT_FAILURE);
    }

    Track track = LoadTrack(argv[1]);
    CartList carts = LoadCarts(track);
    std::sort(carts.begin(), carts.end(), CartLess);

    Cart last_cart_standing = RunSimulation(track, carts);
    std::cout << "Answer: " << last_cart_standing.y << ", "
              << last_cart_standing.x << std::endl;

    exit(EXIT_SUCCESS);
}
