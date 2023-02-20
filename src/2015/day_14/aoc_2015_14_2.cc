#include <algorithm>
#include <climits>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>
#include <vector>

struct Racer {
    std::string name;
    int speed = 0;
    int distance_traveled = 0;
    int sleep_interval_sec = 0;
    int sleep_timer_sec = 0;
    bool is_resting = false;
    int rest_interval_sec = 0;
    int rest_timer_sec = 0;
    int points = 0;
};

Racer ParseRacer(const std::string& racer_specs) {
    std::istringstream iss(racer_specs);
    std::vector<std::string> tokens;
    std::copy(std::istream_iterator<std::string>(iss),
              std::istream_iterator<std::string>(), std::back_inserter(tokens));

    Racer racer = {
        .name = tokens[0],
        .speed = std::stoi(tokens[3]),
        .distance_traveled = 0,
        .sleep_interval_sec = std::stoi(tokens[6]),
        .sleep_timer_sec = 0,
        .is_resting = false,
        .rest_interval_sec = std::stoi(tokens[13]),
        .rest_timer_sec = 0,
        .points = 0,
    };

    return racer;
}

void Step(std::vector<Racer>& racers) {
    for (Racer& racer : racers) {
        if (racer.is_resting) {
            racer.rest_timer_sec++;
            if (racer.rest_timer_sec == racer.rest_interval_sec) {
                racer.rest_timer_sec = 0;
                racer.is_resting = false;
            }
        } else {
            racer.distance_traveled += racer.speed;

            racer.sleep_timer_sec++;
            if (racer.sleep_timer_sec == racer.sleep_interval_sec) {
                racer.sleep_timer_sec = 0;
                racer.is_resting = true;
            }
        }
    }
}

Racer GetWinner(const std::vector<Racer>& racers) {
    return *std::max_element(
        racers.begin(), racers.end(),
        [](const Racer& a, const Racer& b) { return (a.points < b.points); });
}

void UpdatePoints(std::vector<Racer>& racers) {
    int max_distance = INT_MIN;
    for (const Racer& racer : racers) {
        max_distance = std::max(max_distance, racer.distance_traveled);
    }

    for (Racer& racer : racers) {
        if (racer.distance_traveled == max_distance) {
            racer.points++;
        }
    }
}

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "error: missing puzzle input file" << std::endl;
        exit(EXIT_FAILURE);
    }

    std::ifstream input(argv[1]);
    std::string racer_specs;
    std::vector<Racer> racers;
    while (std::getline(input, racer_specs)) {
        racers.push_back(ParseRacer(racer_specs));
    }

    const int kRaceTimeSec = 2503;
    for (int i = 0; i < kRaceTimeSec; ++i) {
        Step(racers);
        UpdatePoints(racers);
    }

    Racer winner = GetWinner(racers);
    std::cout << "Answer: " << winner.name << ", " << winner.points
              << std::endl;

    exit(EXIT_SUCCESS);
}
