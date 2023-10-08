#include <algorithm>
#include <climits>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <iterator>
#include <sstream>
#include <vector>

struct Item;
using ItemList = std::vector<Item>;
using ItemCombo = std::vector<ItemList>;

struct Item {
  std::string name;
  int cost = 0;
  int damage = 0;
  int armor = 0;

  Item(const std::string& name_, int cost_, int damage_, int armor_)
      : name(name_), cost(cost_), damage(damage_), armor(armor_) {}
};

struct Character {
  int hitpoints = 0;
  int total_damage = 0;
  int total_defense = 0;
};

int AttackDamage(const Character& attacker, const Character& defender) {
  int damage = attacker.total_damage - defender.total_defense;
  return (damage <= 0) ? 1 : damage;
}

bool CanWinBattle(Character player, Character enemy) {
  bool players_turn = true;
  while ((player.hitpoints > 0) && (enemy.hitpoints > 0)) {
    if (players_turn) {
      enemy.hitpoints -= AttackDamage(player, enemy);
    } else {
      player.hitpoints -= AttackDamage(enemy, player);
    }
    players_turn = !players_turn;
  }
  return (enemy.hitpoints <= 0);
}

ItemCombo NChooseK(const std::vector<Item>& items, int k) {
  std::vector<int> aux(items.size(), 1);
  std::fill(aux.begin(), aux.begin() + items.size() - k, 0);

  ItemCombo combinations;
  do {
    std::vector<Item> combo;
    for (int i = 0; i < aux.size(); ++i) {
      if (aux[i]) {
        combo.push_back(items[i]);
      }
    }
    combinations.push_back(combo);
  } while (std::next_permutation(aux.begin(), aux.end()));
  return combinations;
}

int GetMaxLosingLoadoutCost(const ItemList& weapons, const ItemList& armors,
                            const ItemList& rings, const Character& enemy) {
  ItemCombo ring_combos = NChooseK(rings, 1);
  ItemCombo ring_combo_2 = NChooseK(rings, 2);
  ring_combos.insert(ring_combos.end(), ring_combo_2.begin(),
                     ring_combo_2.end());
  ring_combos.push_back({Item("None", 0, 0, 0)});

  int max_loadout_cost = INT_MIN;
  for (const Item& weapon : weapons) {
    for (const Item& armor : armors) {
      for (const std::vector<Item>& rings : ring_combos) {
        int curr_loadout_cost = 0;

        Character player;
        player.hitpoints = 100;
        player.total_damage += weapon.damage;
        player.total_defense += armor.armor;
        curr_loadout_cost += weapon.cost + armor.cost;
        for (const Item& ring : rings) {
          player.total_damage += ring.damage;
          player.total_defense += ring.armor;
          curr_loadout_cost += ring.cost;
        }

        if (!CanWinBattle(player, enemy)) {
          max_loadout_cost = std::max(max_loadout_cost, curr_loadout_cost);
        }
      }
    }
  }
  return max_loadout_cost;
}

int ParseEnemyAttribute(const std::string& attr) {
  std::istringstream iss(attr);
  std::vector<std::string> tokens;
  std::copy(std::istream_iterator<std::string>(iss),
            std::istream_iterator<std::string>(), std::back_inserter(tokens));

  return std::stoi(tokens.back());
}

int main(int argc, char** argv) {
  if (argc != 2) {
    std::cerr << "error: missing puzzle input file" << std::endl;
    exit(EXIT_FAILURE);
  }

  std::ifstream input(argv[1]);
  std::string attribute;
  std::vector<std::string> attributes;
  while (std::getline(input, attribute)) {
    attributes.push_back(attribute);
  }

  Character enemy = {
      .hitpoints = ParseEnemyAttribute(attributes[0]),
      .total_damage = ParseEnemyAttribute(attributes[1]),
      .total_defense = ParseEnemyAttribute(attributes[2]),
  };

  ItemList weapons = {
      Item("Dagger", 8, 4, 0),     Item("Shortsword", 10, 5, 0),
      Item("Warhammer", 25, 6, 0), Item("Longsword", 40, 7, 0),
      Item("Greataxe", 74, 8, 0),
  };
  ItemList armors = {
      Item("None", 0, 0, 0),        Item("Leather", 13, 0, 1),
      Item("Chainmail", 31, 0, 2),  Item("Splintmail", 53, 0, 3),
      Item("Bandedmail", 75, 0, 4), Item("Platemail", 102, 0, 5),
  };
  ItemList rings = {
      Item("Damage +1", 25, 1, 0),  Item("Damage +2", 50, 2, 0),
      Item("Damage +3", 100, 3, 0), Item("Defense +1", 20, 0, 1),
      Item("Defense +2", 40, 0, 2), Item("Defense +3", 80, 0, 3),
  };

  std::cout << "Answer: "
            << GetMaxLosingLoadoutCost(weapons, armors, rings, enemy)
            << std::endl;

  exit(EXIT_SUCCESS);
}
