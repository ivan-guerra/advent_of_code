#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>
#include <vector>

std::vector<int> SimulateMarbleGame(int num_players, int num_marbles) {
    std::vector<int> scores(num_players, 0);
    std::vector<int> marbles(1, 0);
    int curr_marble = 0;
    int player_idx = 0;
    for (int i = 1; i <= num_marbles; ++i) {
        if (0 == (i % 23)) {
            scores[player_idx] += i;

            int m1 = (curr_marble - 7 + marbles.size()) % marbles.size();
            scores[player_idx] += marbles[m1];

            if (m1 == (marbles.size() - 1)) {
                marbles.erase(marbles.begin() + m1);
                curr_marble = marbles.size() - 1;
            } else {
                marbles.erase(marbles.begin() + m1);
                curr_marble = m1;
            }
        } else {
            int m1 = (curr_marble + 1) % marbles.size();
            int m2 = (curr_marble + 2) % marbles.size();
            if (m1 == m2) {
                marbles.push_back(i);
                curr_marble++;
            } else if (m1 > m2) {
                marbles.push_back(i);
                curr_marble = marbles.size() - 1;
            } else {
                marbles.insert(marbles.begin() + m2, i);
                curr_marble = m2;
            }
        }
        player_idx = (player_idx + 1) % num_players;
    }
    return scores;
}

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "error: missing puzzle input file" << std::endl;
        exit(EXIT_FAILURE);
    }

    std::ifstream input(argv[1]);
    std::string line;
    std::getline(input, line);
    std::istringstream iss(line);
    std::vector<std::string> words;
    std::copy(std::istream_iterator<std::string>(iss),
              std::istream_iterator<std::string>(), std::back_inserter(words));

    int num_players = std::stoi(words[0]);
    int num_marbles = std::stoi(words[6]);
    std::vector<int> scores = SimulateMarbleGame(num_players, num_marbles);

    std::cout << "Answer: " << *std::max_element(scores.begin(), scores.end())
              << std::endl;

    exit(EXIT_SUCCESS);
}
