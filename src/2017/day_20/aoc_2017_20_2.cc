#include <algorithm>
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

bool IsValidParticle(const Particle& p) { return (p.id >= 0); }

void MarkDuplicateParticles(std::vector<Particle>& particles) {
  for (int i = 0; i < particles.size(); ++i) {
    bool has_collision = false;
    if (IsValidParticle(particles[i])) {
      for (int j = i + 1; j < particles.size(); ++j) {
        if (IsValidParticle(particles[j])) {
          if (particles[i].position == particles[j].position) {
            has_collision = true;
            particles[j].id = -1;
          }
        }
      }
    }
    if (has_collision) {
      particles[i].id = -1;
    }
  }
}

int NumParticlesAfterCollisionResolution(std::vector<Particle>& particles,
                                         int num_steps) {
  while (num_steps--) {
    for (Particle& particle : particles) {
      if (IsValidParticle(particle)) {
        UpdateParticle(particle);
      }
    }
    MarkDuplicateParticles(particles);
  }
  return std::count_if(particles.begin(), particles.end(), IsValidParticle);
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
            << NumParticlesAfterCollisionResolution(particles, kNumIterations)
            << std::endl;

  exit(EXIT_SUCCESS);
}
