#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include <unordered_set>

std::string Stringify(int x, int y) {
    std::ostringstream os;
    os << x << ',' << y;
    return os.str();
}

std::string MakeMove(char move, int& x, int& y) {
    switch (move) {
        case '^':
            y -= 1;
            break;
        case 'v':
            y += 1;
            break;
        case '<':
            x -= 1;
            break;
        case '>':
            x += 1;
            break;
    }
    return Stringify(x, y);
}

int GetNumUniqueDeliveries(const std::string& moves) {
    std::unordered_set<std::string> history;
    history.insert(Stringify(0, 0));

    int santa_x = 0;
    int santa_y = 0;
    int robo_x = 0;
    int robo_y = 0;
    for (int i = 0; i < moves.size(); ++i) {
        if (i & 1) {
            history.insert(MakeMove(moves[i], robo_x, robo_y));
        } else {
            history.insert(MakeMove(moves[i], santa_x, santa_y));
        }
    }
    return history.size();
}

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "error: missing puzzle input file" << std::endl;
        exit(EXIT_FAILURE);
    }

    std::ifstream input(argv[1]);
    std::string moves;
    std::getline(input, moves);

    std::cout << "Answer: " << GetNumUniqueDeliveries(moves) << std::endl;

    exit(EXIT_SUCCESS);
}
