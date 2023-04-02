#include <cstdint>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <iterator>
#include <memory>
#include <queue>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

class DuetMachine;
using DuetMachinePtr = std::shared_ptr<DuetMachine>;

enum ProgramStatus {
    kRunning,
    kWaiting,
    kComplete,
};

enum InstructionType {
    kSnd,
    kSet,
    kAdd,
    kMul,
    kMod,
    kRcv,
    kJmp,
};

struct Instruction {
    InstructionType type;
    std::string arg1;
    std::string arg2;
};

class DuetMachine {
   public:
    DuetMachine() = delete;
    DuetMachine(int id, const std::vector<Instruction>& instructions);
    ~DuetMachine() = default;

    DuetMachine(const DuetMachine&) = default;
    DuetMachine(DuetMachine&&) = default;
    DuetMachine& operator=(const DuetMachine&) = default;
    DuetMachine& operator=(DuetMachine&&) = default;

    void Step();
    void Recv(int value) { recved_msgs_.push(value); }
    void SetPeer(DuetMachinePtr peer) { peer_ = peer; }
    int GetSendCount() const { return send_ctr_; }
    ProgramStatus GetStatus() const { return status_; }

   private:
    using RegisterMap = std::unordered_map<std::string, int64_t>;

    int EvaluateArg(const std::string& arg) const;

    int id_;
    int ip_;
    int send_ctr_;
    ProgramStatus status_;
    std::vector<Instruction> instructions_;
    RegisterMap registers_;
    std::queue<int> recved_msgs_;
    DuetMachinePtr peer_;
};

int DuetMachine::EvaluateArg(const std::string& arg) const {
    if (registers_.count(arg)) {
        return registers_.at(arg);
    }
    return std::stoi(arg);
}

DuetMachine::DuetMachine(int id, const std::vector<Instruction>& instructions)
    : id_(id),
      ip_(0),
      send_ctr_(0),
      status_(ProgramStatus::kRunning),
      instructions_(instructions) {
    registers_["p"] = id_;
}

void DuetMachine::Step() {
    if ((ip_ < 0) || (ip_ >= instructions_.size())) {
        status_ = ProgramStatus::kComplete;
        return;
    }

    const Instruction& inst = instructions_[ip_];
    switch (inst.type) {
        case kSnd:
            peer_->Recv(EvaluateArg(inst.arg1));
            send_ctr_++;
            ip_++;
            break;
        case kSet:
            registers_[inst.arg1] = EvaluateArg(inst.arg2);
            ip_++;
            break;
        case kAdd:
            registers_[inst.arg1] += EvaluateArg(inst.arg2);
            ip_++;
            break;
        case kMul:
            registers_[inst.arg1] *= EvaluateArg(inst.arg2);
            ip_++;
            break;
        case kMod:
            registers_[inst.arg1] %= EvaluateArg(inst.arg2);
            ip_++;
            break;
        case kRcv:
            if (recved_msgs_.empty()) {
                status_ = ProgramStatus::kWaiting;
                return;
            } else {
                int recved_val = recved_msgs_.front();
                recved_msgs_.pop();
                registers_[inst.arg1] = recved_val;
                ip_++;
            }
            break;
        case kJmp:
            if (EvaluateArg(inst.arg1) > 0) {
                ip_ += EvaluateArg(inst.arg2);
            } else {
                ip_++;
            }
            break;
    }
    status_ = ProgramStatus::kRunning;
}

Instruction ParseInstruction(const std::string& text) {
    std::istringstream iss(text);
    std::vector<std::string> tokens;
    std::copy(std::istream_iterator<std::string>(iss),
              std::istream_iterator<std::string>(), std::back_inserter(tokens));

    Instruction inst;
    if (tokens[0] == "snd") {
        inst.type = InstructionType::kSnd;
        inst.arg1 = tokens[1];
    } else if (tokens[0] == "set") {
        inst.type = InstructionType::kSet;
        inst.arg1 = tokens[1];
        inst.arg2 = tokens[2];
    } else if (tokens[0] == "add") {
        inst.type = InstructionType::kAdd;
        inst.arg1 = tokens[1];
        inst.arg2 = tokens[2];
    } else if (tokens[0] == "mul") {
        inst.type = InstructionType::kMul;
        inst.arg1 = tokens[1];
        inst.arg2 = tokens[2];
    } else if (tokens[0] == "mod") {
        inst.type = InstructionType::kMod;
        inst.arg1 = tokens[1];
        inst.arg2 = tokens[2];
    } else if (tokens[0] == "rcv") {
        inst.type = InstructionType::kRcv;
        inst.arg1 = tokens[1];
    } else if (tokens[0] == "jgz") {
        inst.type = InstructionType::kJmp;
        inst.arg1 = tokens[1];
        inst.arg2 = tokens[2];
    }
    return inst;
}

int GetProg1SendCount(const std::vector<Instruction>& instructions) {
    DuetMachinePtr prog0 = std::make_shared<DuetMachine>(0, instructions);
    DuetMachinePtr prog1 = std::make_shared<DuetMachine>(1, instructions);
    prog0->SetPeer(prog1);
    prog1->SetPeer(prog0);

    while ((prog0->GetStatus() == ProgramStatus::kRunning) ||
           (prog1->GetStatus() == ProgramStatus::kRunning)) {
        prog0->Step();
        prog1->Step();
    }
    return prog1->GetSendCount();
}

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "error: missing puzzle input file" << std::endl;
        exit(EXIT_FAILURE);
    }

    std::ifstream input(argv[1]);
    std::string text;
    std::vector<Instruction> instructions;
    while (std::getline(input, text)) {
        instructions.push_back(ParseInstruction(text));
    }

    std::cout << "Answer: " << GetProg1SendCount(instructions) << std::endl;

    exit(EXIT_SUCCESS);
}
