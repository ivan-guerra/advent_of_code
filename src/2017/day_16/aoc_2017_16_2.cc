#include <cstdlib>
#include <fstream>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>
#include <vector>

enum DanceMoveType {
    kSpin,
    kExchange,
    kPartner,
};

struct DanceMove {
    DanceMoveType type;
    std::string arg1;
    std::string arg2;
};

std::vector<DanceMove> ParseDanceMoves(const std::string& text) {
    std::istringstream iss(text);
    std::vector<std::string> tokens;
    std::copy(std::istream_iterator<std::string>(iss),
              std::istream_iterator<std::string>(), std::back_inserter(tokens));

    std::vector<DanceMove> moves;
    for (const std::string& token : tokens) {
        DanceMove move;
        switch (token[0]) {
            case 's': {
                move.type = DanceMoveType::kSpin;
                move.arg1 = token.substr(1);
                break;
            }
            case 'x': {
                int slash_idx = token.find("/");
                move.type = DanceMoveType::kExchange;
                move.arg1 = token.substr(1, slash_idx - 1);
                move.arg2 = token.substr(slash_idx + 1);
                break;
            }
            case 'p': {
                int slash_idx = token.find("/");
                move.type = DanceMoveType::kPartner;
                move.arg1 = token.substr(1, slash_idx - 1);
                move.arg2 = token.substr(slash_idx + 1);
                break;
            }
        }
        moves.push_back(move);
    }
    return moves;
}

void ExecSpinMove(std::string& programs, const DanceMove& spin) {
    int i = programs.size() - std::stoi(spin.arg1);
    std::string group = programs.substr(i);
    programs.erase(programs.begin() + i, programs.end());
    programs.insert(0, group);
}

void ExecExchange(std::string& programs, const DanceMove& exchange) {
    std::swap(programs[std::stoi(exchange.arg1)],
              programs[std::stoi(exchange.arg2)]);
}

void ExecPartnerExchange(std::string& programs, const DanceMove& exchange) {
    int i = programs.find(exchange.arg1);
    int j = programs.find(exchange.arg2);
    std::swap(programs[i], programs[j]);
}

void ExecuteDance(std::string& programs, const std::vector<DanceMove>& moves) {
    for (const DanceMove& move : moves) {
        switch (move.type) {
            case kSpin:
                ExecSpinMove(programs, move);
                break;
            case kExchange:
                ExecExchange(programs, move);
                break;
            case kPartner:
                ExecPartnerExchange(programs, move);
                break;
        }
    }
}

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "error: missing puzzle input file" << std::endl;
        exit(EXIT_FAILURE);
    }

    std::ifstream input(argv[1]);
    std::string text;
    std::getline(input, text);

    std::string programs = "abcdefghijklmnop";
    std::vector<DanceMove> moves = ParseDanceMoves(text);

    /* Compute the size of the dance cycle. */
    ExecuteDance(programs, moves);
    int cycle_size = 1;
    while (programs != "abcdefghijklmnop") {
        ExecuteDance(programs, moves);
        cycle_size++;
    }

    /* Shortcut our way over the 1 billion dances using the modulo operator. */
    int j = 1000000000 % cycle_size;
    while (j--) {
        ExecuteDance(programs, moves);
    }

    std::cout << "Answer: " << programs << std::endl;

    exit(EXIT_SUCCESS);
}
