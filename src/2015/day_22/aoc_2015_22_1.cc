#include <cstdlib>
#include <fstream>
#include <iostream>
#include <iterator>
#include <queue>
#include <sstream>
#include <unordered_map>
#include <vector>

enum SpellType {
    kMagicMissile,
    kDrain,
    kShield,
    kPoison,
    kRecharge,
};

struct Wizard {
    int hitpoints = 0;
    int mana = 0;
    int armor = 0;

    int missile_damage = 0;

    int drain_damage = 0;
    int drain_heal = 0;

    int shield_armor = 0;
    int shield_duration = 0;
    int shield_timer = 0;

    int poison_damage = 0;
    int poison_duration = 0;
    int poison_timer = 0;

    int recharge_mana = 0;
    int recharge_duration = 0;
    int recharge_timer = 0;
};

struct Boss {
    int hitpoints = 0;
    int damage = 0;
};

class BattleSimulation {
   public:
    BattleSimulation() = delete;
    BattleSimulation(const Wizard& wizard, const Boss& boss);
    ~BattleSimulation() = default;
    BattleSimulation(const BattleSimulation&) = default;
    BattleSimulation& operator=(const BattleSimulation&) = default;
    BattleSimulation(BattleSimulation&&) = default;
    BattleSimulation& operator=(BattleSimulation&&) = default;

    bool WizardWins() const {
        return ((wizard_.hitpoints > 0) && (wizard_.mana > 0) &&
                (boss_.hitpoints <= 0));
    }
    bool BossWins() const {
        return ((boss_.hitpoints > 0) &&
                (wizard_.hitpoints <= 0 || wizard_.mana <= 0));
    }

    void Step(SpellType spell);
    void WizardAttacksBoss(SpellType spell);
    void BossAttacksWizard();
    int GetTotalManaSpent() const { return total_mana_spent_; }

   private:
    using SpellCostTable = std::unordered_map<SpellType, int>;
    static const SpellCostTable kSpellCosts;

    void ApplyEffects();
    void DecrementEffectTimers();

    int total_mana_spent_;
    Wizard wizard_;
    Boss boss_;
};

const BattleSimulation::SpellCostTable BattleSimulation::kSpellCosts = {
    {kMagicMissile, 53}, {kDrain, 73},     {kShield, 113},
    {kPoison, 173},      {kRecharge, 229},
};

void BattleSimulation::ApplyEffects() {
    if (wizard_.poison_timer) {
        boss_.hitpoints -= wizard_.poison_damage;
    }
    if (wizard_.recharge_timer) {
        wizard_.mana += wizard_.recharge_mana;
    }
}

void BattleSimulation::DecrementEffectTimers() {
    if (wizard_.poison_timer > 0) {
        wizard_.poison_timer--;
    }
    if (wizard_.shield_timer > 0) {
        wizard_.shield_timer--;
        if (0 == wizard_.shield_timer) {
            wizard_.armor = 0;
        }
    }
    if (wizard_.recharge_timer > 0) {
        wizard_.recharge_timer--;
    }
}

BattleSimulation::BattleSimulation(const Wizard& wizard, const Boss& boss)
    : total_mana_spent_(0), wizard_(wizard), boss_(boss) {}

void BattleSimulation::WizardAttacksBoss(SpellType spell) {
    ApplyEffects();

    switch (spell) {
        case SpellType::kMagicMissile:
            boss_.hitpoints -= wizard_.missile_damage;
            break;
        case SpellType::kDrain:
            boss_.hitpoints -= wizard_.drain_damage;
            wizard_.hitpoints += wizard_.drain_heal;
            break;
        case SpellType::kShield:
            wizard_.armor += wizard_.shield_armor;
            wizard_.shield_timer = wizard_.shield_duration + 1;
            break;
        case SpellType::kPoison:
            wizard_.poison_timer = wizard_.poison_duration + 1;
            break;
        case SpellType::kRecharge:
            wizard_.recharge_timer = wizard_.recharge_duration + 1;
            break;
    }
    wizard_.mana -= kSpellCosts.at(spell);
    total_mana_spent_ += kSpellCosts.at(spell);

    DecrementEffectTimers();
}

void BattleSimulation::BossAttacksWizard() {
    ApplyEffects();

    if (boss_.hitpoints <= 0) {
        return;
    }

    int hitsplat = boss_.damage - wizard_.armor;
    wizard_.hitpoints -= (hitsplat <= 0) ? 1 : hitsplat;

    DecrementEffectTimers();
}

int ParseBossAttribute(const std::string& attr) {
    std::istringstream iss(attr);
    std::vector<std::string> tokens;
    std::copy(std::istream_iterator<std::string>(iss),
              std::istream_iterator<std::string>(), std::back_inserter(tokens));

    return std::stoi(tokens.back());
}

int FindMinManaCost(const Wizard& wizard, const Boss& boss) {
    BattleSimulation sim(wizard, boss);
    std::queue<std::pair<SpellType, BattleSimulation>> nodes;

    nodes.push(std::make_pair(SpellType::kMagicMissile, sim));
    nodes.push(std::make_pair(SpellType::kDrain, sim));
    nodes.push(std::make_pair(SpellType::kShield, sim));
    nodes.push(std::make_pair(SpellType::kPoison, sim));
    nodes.push(std::make_pair(SpellType::kRecharge, sim));

    while (!nodes.empty()) {
        int level = nodes.size();
        for (int i = 0; i < level; ++i) {
            auto curr_node = nodes.front();
            nodes.pop();

            curr_node.second.WizardAttacksBoss(curr_node.first);
            curr_node.second.BossAttacksWizard();
            if (curr_node.second.WizardWins()) {
                return curr_node.second.GetTotalManaSpent();
            } else if (curr_node.second.BossWins()) {
                // Dead end, boss kills us with this spell sequence.
            } else {
                nodes.push(
                    std::make_pair(SpellType::kMagicMissile, curr_node.second));
                nodes.push(std::make_pair(SpellType::kDrain, curr_node.second));
                nodes.push(
                    std::make_pair(SpellType::kShield, curr_node.second));
                nodes.push(
                    std::make_pair(SpellType::kPoison, curr_node.second));
                nodes.push(
                    std::make_pair(SpellType::kRecharge, curr_node.second));
            }
        }
    }
    return -1; /* Means it is impossible for the wizard to win! */
}

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "error: missing puzzle input file" << std::endl;
        exit(EXIT_FAILURE);
    }

    std::ifstream input(argv[1]);
    std::string boss_attribute;
    std::vector<std::string> attributes;
    while (std::getline(input, boss_attribute)) {
        attributes.push_back(boss_attribute);
    }

    Wizard gandalf = {
        .hitpoints = 50,
        .mana = 500,
        .armor = 0,
        .missile_damage = 4,
        .drain_damage = 2,
        .drain_heal = 2,
        .shield_armor = 7,
        .shield_duration = 6,
        .shield_timer = 0,
        .poison_damage = 3,
        .poison_duration = 6,
        .poison_timer = 0,
        .recharge_mana = 101,
        .recharge_duration = 5,
        .recharge_timer = 0,
    };
    Boss sauron = {
        .hitpoints = ParseBossAttribute(attributes[0]),
        .damage = ParseBossAttribute(attributes[1]),
    };

    std::cout << "Answer: " << FindMinManaCost(gandalf, sauron) << std::endl;

    exit(EXIT_SUCCESS);
}
