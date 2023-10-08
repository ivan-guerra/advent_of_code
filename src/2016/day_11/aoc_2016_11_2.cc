#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <queue>
#include <string>
#include <unordered_set>
#include <vector>

enum class AssemblyItemType {
  kNone,
  kMicrochip,
  kGenerator,
};

struct AssemblyItem {
  AssemblyItemType type = AssemblyItemType::kNone;
  std::string element = "none";

  friend bool operator==(const AssemblyItem& a, const AssemblyItem& b) {
    return (a.type == b.type) && (a.element == b.element);
  }
};

class AssemblySim {
 public:
  using Floor = std::vector<AssemblyItem>;
  using Floors = std::vector<Floor>;

  enum class ElevatorDir {
    kUp,
    kDown,
  };

  AssemblySim() = delete;
  AssemblySim(const Floors& floors) : level_(0), floors_(floors) {}
  ~AssemblySim() = default;
  AssemblySim(const AssemblySim&) = default;
  AssemblySim(AssemblySim&&) = default;

  AssemblySim& operator=(const AssemblySim&) = default;
  AssemblySim& operator=(AssemblySim&&) = default;

  const Floors& GetFloors() const { return floors_; }

  std::vector<int> GetCurrFloorItemIndices() const {
    std::vector<int> item_indices;
    for (int i = 0; i < floors_[level_].size(); ++i) {
      if (floors_[level_][i].type != AssemblyItemType::kNone) {
        item_indices.push_back(i);
      }
    }
    return item_indices;
  }

  bool IsInvalid() const {
    for (int i = 0; i < floors_[level_].size(); ++i) {
      if ((floors_[level_][i].type == AssemblyItemType::kMicrochip) &&
          FloorHasGenerator()) {
        bool found_partner_generator = false;
        for (int j = 0; j < floors_[level_].size(); ++j) {
          AssemblyItemType type = floors_[level_][j].type;
          std::string element = floors_[level_][j].element;
          if ((type == AssemblyItemType::kGenerator) &&
              (element == floors_[level_][i].element)) {
            found_partner_generator = true;
            break;
          }
        }
        if (!found_partner_generator) {
          return true;
        }
      }
    }
    return false;
  }
  bool IsComplete() const {
    for (int i = 0; i < floors_.size() - 1; i++) {
      bool floor_is_clear = std::all_of(
          floors_[i].begin(), floors_[i].end(), [](const AssemblyItem& ai) {
            return (ai.type == AssemblyItemType::kNone);
          });
      if (!floor_is_clear) {
        return false;
      }
    }
    return true;
  }
  bool FloorsBelowAreEmpty() const {
    for (int i = level_ - 1; i >= 0; --i) {
      if (!FloorIsEmpty(i)) {
        return false;
      }
    }
    return true;
  }

  bool Step(ElevatorDir dir, int item_index) {
    if (!StepItem(dir, item_index)) {
      return false;
    }
    level_ = (ElevatorDir::kUp == dir) ? (level_ + 1) : (level_ - 1);
    return true;
  }
  bool Step(ElevatorDir dir, int item1_index, int item2_index) {
    if (!StepItem(dir, item1_index) || !StepItem(dir, item2_index)) {
      return false;
    }
    level_ = (ElevatorDir::kUp == dir) ? (level_ + 1) : (level_ - 1);
    return true;
  }

  friend bool operator==(const AssemblySim& a, const AssemblySim& b) {
    return (a.GetFloors() == b.GetFloors()) && (a.level_ == b.level_);
  }

 private:
  bool StepItem(ElevatorDir dir, int item_index) {
    if (ElevatorDir::kUp == dir) {
      if ((level_ + 1) >= floors_.size()) {
        return false;
      }
      std::swap(floors_[level_][item_index], floors_[level_ + 1][item_index]);
    } else {
      if ((level_ - 1) < 0) {
        return false;
      }
      std::swap(floors_[level_][item_index], floors_[level_ - 1][item_index]);
    }
    return true;
  }

  bool FloorHasGenerator() const {
    return std::any_of(floors_[level_].begin(), floors_[level_].end(),
                       [](const AssemblyItem& ai) {
                         return (ai.type == AssemblyItemType::kGenerator);
                       });
  }

  bool FloorIsEmpty(int i) const {
    return std::all_of(floors_[i].begin(), floors_[i].end(),
                       [](const AssemblyItem& ai) {
                         return (ai.type == AssemblyItemType::kNone);
                       });
  }

  int level_;
  Floors floors_;
};

struct AssemblySimHash {
  std::size_t operator()(const AssemblySim& sim) const {
    std::string state_str;
    for (const AssemblySim::Floor& floor : sim.GetFloors()) {
      for (const AssemblyItem& ai : floor) {
        state_str += ai.element;
      }
    }
    return std::hash<std::string>{}(state_str);
  }
};

std::vector<std::vector<int>> NChooseK(const std::vector<int>& values, int k) {
  if (values.size() < k) {
    return {};
  }

  std::vector<int> aux(values.size(), 1);
  std::fill(aux.begin(), aux.begin() + values.size() - k, 0);

  std::vector<std::vector<int>> combinations;
  do {
    std::vector<int> combo;
    for (int i = 0; i < aux.size(); ++i) {
      if (aux[i]) {
        combo.push_back(values[i]);
      }
    }
    combinations.push_back(combo);
  } while (std::next_permutation(aux.begin(), aux.end()));
  return combinations;
}

int GetMinStepsToCompleteAssembly(const AssemblySim::Floors& floors) {
  AssemblySim init(floors);
  std::queue<AssemblySim> sims;
  sims.push(init);
  std::unordered_set<AssemblySim, AssemblySimHash> visited;

  int steps = 0;
  while (true) {
    int num_nodes = sims.size();
    for (int i = 0; i < num_nodes; ++i) {
      AssemblySim curr = sims.front();
      sims.pop();

      if (visited.count(curr)) {
        continue;
      }
      visited.insert(curr);

      if (curr.IsComplete()) {
        return steps;
      }

      auto combos = NChooseK(curr.GetCurrFloorItemIndices(), 1);
      auto double_item_combos = NChooseK(curr.GetCurrFloorItemIndices(), 2);
      combos.insert(combos.end(), double_item_combos.begin(),
                    double_item_combos.end());

      for (const std::vector<int>& combo : combos) {
        AssemblySim candidate_up = curr;
        AssemblySim candidate_down = curr;
        if (combo.size() == 2) {
          if (candidate_up.Step(AssemblySim::ElevatorDir::kUp, combo[0],
                                combo[1]) &&
              !candidate_up.IsInvalid()) {
            sims.push(candidate_up);
          }
          if (!candidate_down.FloorsBelowAreEmpty() &&
              candidate_down.Step(AssemblySim::ElevatorDir::kDown, combo[0],
                                  combo[1]) &&
              !candidate_down.IsInvalid()) {
            sims.push(candidate_down);
          }
        } else {
          if (candidate_up.Step(AssemblySim::ElevatorDir::kUp, combo[0]) &&
              !candidate_up.IsInvalid()) {
            sims.push(candidate_up);
          }
          if (!candidate_down.FloorsBelowAreEmpty() &&
              candidate_down.Step(AssemblySim::ElevatorDir::kDown, combo[0]) &&
              !candidate_down.IsInvalid()) {
            sims.push(candidate_down);
          }
        }
      }
    }
    steps++;
  }
  return -1;
}

int main() {
  const int kNumFloors = 4;
  const int kNumItemsPerFloor = 14;
  AssemblySim::Floors floors(kNumFloors,
                             std::vector<AssemblyItem>(kNumItemsPerFloor));

  floors[0][0] = {
      .type = AssemblyItemType::kGenerator,
      .element = "strontium",
  };
  floors[0][1] = {
      .type = AssemblyItemType::kMicrochip,
      .element = "strontium",
  };
  floors[0][2] = {
      .type = AssemblyItemType::kGenerator,
      .element = "plutonium",
  };
  floors[0][3] = {
      .type = AssemblyItemType::kMicrochip,
      .element = "plutonium",
  };
  floors[0][10] = {
      .type = AssemblyItemType::kGenerator,
      .element = "elerium",
  };
  floors[0][11] = {
      .type = AssemblyItemType::kMicrochip,
      .element = "elerium",
  };
  floors[0][12] = {
      .type = AssemblyItemType::kGenerator,
      .element = "dilithium",
  };
  floors[0][13] = {
      .type = AssemblyItemType::kMicrochip,
      .element = "dilithium",
  };

  floors[1][4] = {
      .type = AssemblyItemType::kGenerator,
      .element = "thulium",
  };
  floors[1][5] = {
      .type = AssemblyItemType::kGenerator,
      .element = "ruthenium",
  };
  floors[1][6] = {
      .type = AssemblyItemType::kMicrochip,
      .element = "ruthenium",
  };
  floors[1][7] = {
      .type = AssemblyItemType::kGenerator,
      .element = "curium",
  };
  floors[1][8] = {
      .type = AssemblyItemType::kMicrochip,
      .element = "curium",
  };

  floors[2][9] = {
      .type = AssemblyItemType::kMicrochip,
      .element = "thulium",
  };

  std::cout << "Answer: " << GetMinStepsToCompleteAssembly(floors) << std::endl;

  exit(EXIT_SUCCESS);
}
