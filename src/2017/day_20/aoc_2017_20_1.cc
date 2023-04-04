#include <cmath>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

using Vector3D = std::vector<int>;

struct Particle {
    int id = -1;
    Vector3D position;
    Vector3D velocity;
    Vector3D accel;
};

Particle ParseParticle(int id, const std::string& text) {
    std::istringstream iss(text);
    std::vector<std::string> tokens;
    std::copy(std::istream_iterator<std::string>(iss),
              std::istream_iterator<std::string>(), std::back_inserter(tokens));

    auto ParseComponents = [](const std::string& csv) {
        std::vector<int> components;
        std::istringstream iss(csv);
        while (iss.good()) {
            std::string component;
            std::getline(iss, component, ',');
            components.push_back(std::stoi(component));
        }
        return components;
    };

    std::string pos_str = tokens[0].substr(3, tokens[0].size() - 5);
    std::string vel_str = tokens[1].substr(3, tokens[1].size() - 5);
    std::string acc_str = tokens[2].substr(3, tokens[2].size() - 4);

    Particle particle = {
        .id = id,
        .position = ParseComponents(pos_str),
        .velocity = ParseComponents(vel_str),
        .accel = ParseComponents(acc_str),
    };
    return particle;
}

void UpdateParticle(Particle& particle) {
    particle.velocity[0] += particle.accel[0];
    particle.velocity[1] += particle.accel[1];
    particle.velocity[2] += particle.accel[2];

    particle.position[0] += particle.velocity[0];
    particle.position[1] += particle.velocity[1];
    particle.position[2] += particle.velocity[2];
}

void TallyParticlesClosestToOrigin(const std::vector<Particle>& particles,
                                   std::unordered_map<int, int>& tally) {
    auto ComputeParticleDist = [](const Particle& p) {
        return (std::abs(p.position[0]) + std::abs(p.position[1]) +
                std::abs(p.position[2]));
    };

    /* Find the minimum distance to the origin on this tick. */
    int min_dist = ComputeParticleDist(particles[0]);
    for (int i = 1; i < particles.size(); ++i) {
        min_dist = std::min(min_dist, ComputeParticleDist(particles[i]));
    }

    /* Up the count for those particles who share the min dist to the origin. */
    for (const Particle& p : particles) {
        if (ComputeParticleDist(p) == min_dist) {
            tally[p.id]++;
        }
    }
}

int GetClosestParticleId(const std::unordered_map<int, int>& tally) {
    int max_score = 0;
    for (const auto& [id, score] : tally) {
        max_score = std::max(max_score, score);
    }

    for (const auto& [id, score] : tally) {
        if (score == max_score) {
            return id;
        }
    }
    return -1; /* unreachable */
}

int EstimateClosestParticle(std::vector<Particle>& particles, int num_steps) {
    std::unordered_map<int, int> tally;
    for (const Particle& p : particles) {
        tally[p.id] = 0;
    }

    while (num_steps--) {
        for (Particle& particle : particles) {
            UpdateParticle(particle);
        }
        TallyParticlesClosestToOrigin(particles, tally);
    }
    return GetClosestParticleId(tally);
}

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "error: missing puzzle input file" << std::endl;
        exit(EXIT_FAILURE);
    }

    std::ifstream input(argv[1]);
    std::string line;
    std::vector<Particle> particles;
    int id = 0;
    while (std::getline(input, line)) {
        particles.push_back(ParseParticle(id, line));
        id++;
    }

    const int kNumIterations = 10000;
    std::cout << "Answer: "
              << EstimateClosestParticle(particles, kNumIterations)
              << std::endl;

    exit(EXIT_SUCCESS);
}
