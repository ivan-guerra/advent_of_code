#include <cstdlib>
#include <fstream>
#include <iostream>
#include <stack>
#include <string>
#include <unordered_set>
#include <vector>

struct Component;
struct ComponentHash;

using ComponentSet = std::unordered_set<Component, ComponentHash>;

struct Component {
  int port_a;
  int port_b;
  int strength;

  Component(int port_a_ = 0, int port_b_ = 0, int strength_ = 0)
      : port_a(port_a_), port_b(port_b_), strength(strength_) {}

  friend bool operator==(const Component& a, const Component& b) {
    return ((a.port_a == b.port_a) && (a.port_b == b.port_b));
  }
};

struct ComponentHash {
  std::size_t operator()(const Component& comp) const {
    std::string comp_str =
        std::to_string(comp.port_a) + "," + std::to_string(comp.port_b);
    return std::hash<std::string>{}(comp_str);
  }
};

bool IsCompatible(const Component& old_comp, const Component& new_comp) {
  return (new_comp.port_a == old_comp.port_b) ||
         (new_comp.port_b == old_comp.port_b);
}

Component ParseComponent(const std::string text) {
  int slash_idx = text.find("/");
  Component comp(std::stoi(text.substr(0, slash_idx)),
                 std::stoi(text.substr(slash_idx + 1)));
  comp.strength = comp.port_a + comp.port_b;
  return comp;
}

std::pair<int, int> ComputeMaximums(ComponentSet components, Component comp) {
  std::pair<int, int> maxes = std::make_pair(0, comp.strength);
  for (Component new_comp : components) {
    if (IsCompatible(comp, new_comp)) {
      ComponentSet new_components = components;
      new_components.erase(new_comp);

      if (comp.port_b != new_comp.port_a) {
        std::swap(new_comp.port_a, new_comp.port_b);
      }
      new_comp.strength = new_comp.port_a + new_comp.port_b + comp.strength;

      std::pair<int, int> new_maxes = ComputeMaximums(new_components, new_comp);
      if (new_maxes.first == maxes.first) {
        maxes.second = std::max(maxes.second, new_maxes.second);
      } else if (new_maxes.first > maxes.first) {
        maxes = new_maxes;
      }
    }
  }
  maxes.first += 1;

  return maxes;
}

int ComputeMaxStrengthTwo(ComponentSet& components) {
  std::vector<Component> start_comps;
  for (const Component& comp : components) {
    if (comp.port_a == 0) {
      start_comps.push_back(comp);
    }
  }

  std::pair<int, int> curr_maxes;
  for (const Component& comp : start_comps) {
    ComponentSet comps = components;
    comps.erase(comp);
    std::pair<int, int> maxes = ComputeMaximums(comps, comp);
    if (maxes.first > curr_maxes.first) {
      curr_maxes = maxes;
    }
  }
  return curr_maxes.second;
}

int main(int argc, char** argv) {
  if (argc != 2) {
    std::cerr << "error: missing puzzle input file" << std::endl;
    exit(EXIT_FAILURE);
  }

  std::ifstream input(argv[1]);
  std::string line;
  ComponentSet components;
  while (std::getline(input, line)) {
    components.insert(ParseComponent(line));
  }

  std::cout << "Answer: " << ComputeMaxStrengthTwo(components) << std::endl;

  exit(EXIT_SUCCESS);
}
