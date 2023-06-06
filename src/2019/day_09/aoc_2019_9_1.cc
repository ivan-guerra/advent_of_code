#include <array>
#include <cstdint>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

using Token = int64_t;
using Tokens = std::vector<Token>;

static const int kTokenBuffSize = 1024 * 1024;

Tokens ReadTokens(const std::string& input, int stream_size = kTokenBuffSize) {
    std::stringstream ss(input);
    std::string token;

    int i = 0;
    Tokens tokens(stream_size, 0);
    while (std::getline(ss, token, ',')) {
        tokens[i++] = std::stoll(token);
    }
    return tokens;
}

class IntCodeInterpreter {
   public:
    IntCodeInterpreter() = delete;
    explicit IntCodeInterpreter(const Tokens& tokens)
        : tokens_(tokens), ip_(0), rbase_(0) {}

    void Run() {
        Statement stmt = ParseStatement();
        while (stmt.opcode != OpCode::kHalt) {
            ExecuteStatement(stmt);
            stmt = ParseStatement();
        }
    }

   private:
    enum ParamMode {
        kPosition = 0,
        kImmediate = 1,
        kRelative = 2,
    };

    struct Param {
        ParamMode mode;
        Token value;
    };

    enum OpCode {
        kAdd = 1,
        kMultiply = 2,
        kInput = 3,
        kOutput = 4,
        kJmpIfTrue = 5,
        kJmpIfFalse = 6,
        kLessThan = 7,
        kEquals = 8,
        kRelBaseOffset = 9,
        kHalt = 99,
    };

    struct Statement {
        OpCode opcode;
        std::vector<Param> params;
    };

    using OpCodeLengths = std::unordered_map<OpCode, int>;
    static const OpCodeLengths kOpLens;

    OpCode GetOpCode(Token code) const {
        constexpr int kNumOpCodeDigits = 2;
        const int kBase10 = 10;

        std::array<Token, kNumOpCodeDigits> digits = {0};
        digits[0] = code % kBase10;
        digits[1] = (code / kBase10) % kBase10;

        return static_cast<OpCode>(digits[1] * kBase10 + digits[0]);
    }

    ParamMode GetParamMode(Token code, int param_num) const {
        const int kOpCodeOffset = 2;
        const int kBase10 = 10;

        int param_offset = kOpCodeOffset + param_num;
        int divisor = 1;
        while (param_offset--) {
            divisor *= kBase10;
        }

        return static_cast<ParamMode>((code / divisor) % kBase10);
    }

    Statement ParseStatement() const {
        Statement stmt;
        stmt.opcode = GetOpCode(tokens_[ip_]);

        int op_len = kOpLens.at(stmt.opcode);
        for (int param_idx = 0; param_idx < op_len; ++param_idx) {
            Param param{.mode = GetParamMode(tokens_[ip_], param_idx),
                        .value = tokens_[ip_ + param_idx + 1]};
            stmt.params.push_back(param);
        }
        return stmt;
    }

    Token FetchParam(const Param& param) {
        if (param.mode == ParamMode::kImmediate) {
            return param.value;
        } else if (param.mode == ParamMode::kRelative) {
            return tokens_[rbase_ + param.value];
        }
        return tokens_[param.value];
    }

    void WriteValue(const Param& address, const Token& value) {
        if (address.mode == ParamMode::kRelative) {
            tokens_[rbase_ + address.value] = value;
        } else {
            tokens_[address.value] = value;
        }
    }

    void ExecuteStatement(const Statement& stmt) {
        switch (stmt.opcode) {
            case kAdd:
                WriteValue(stmt.params[2], FetchParam(stmt.params[0]) +
                                               FetchParam(stmt.params[1]));
                ip_ += kOpLens.at(stmt.opcode) + 1;
                break;
            case kMultiply:
                WriteValue(stmt.params[2], FetchParam(stmt.params[0]) *
                                               FetchParam(stmt.params[1]));
                ip_ += kOpLens.at(stmt.opcode) + 1;
                break;
            case kInput:
                std::cout << "input value: ";
                if (stmt.params[0].mode == ParamMode::kRelative) {
                    std::cin >> tokens_[rbase_ + stmt.params[0].value];
                } else {
                    std::cin >> tokens_[stmt.params[0].value];
                }
                ip_ += kOpLens.at(stmt.opcode) + 1;
                break;
            case kOutput:
                std::cout << "output value: " << FetchParam(stmt.params[0])
                          << std::endl;
                ip_ += kOpLens.at(stmt.opcode) + 1;
                break;
            case kJmpIfTrue:
                if (FetchParam(stmt.params[0])) {
                    ip_ = FetchParam(stmt.params[1]);
                } else {
                    ip_ += kOpLens.at(stmt.opcode) + 1;
                }
                break;
            case kJmpIfFalse:
                if (FetchParam(stmt.params[0]) == 0) {
                    ip_ = FetchParam(stmt.params[1]);
                } else {
                    ip_ += kOpLens.at(stmt.opcode) + 1;
                }
                break;
            case kLessThan:
                if (FetchParam(stmt.params[0]) < FetchParam(stmt.params[1])) {
                    WriteValue(stmt.params[2], 1);
                } else {
                    WriteValue(stmt.params[2], 0);
                }
                ip_ += kOpLens.at(stmt.opcode) + 1;
                break;
            case kEquals:
                if (FetchParam(stmt.params[0]) == FetchParam(stmt.params[1])) {
                    WriteValue(stmt.params[2], 1);
                } else {
                    WriteValue(stmt.params[2], 0);
                }
                ip_ += kOpLens.at(stmt.opcode) + 1;
                break;
            case kRelBaseOffset:
                rbase_ += FetchParam(stmt.params[0]);
                ip_ += kOpLens.at(stmt.opcode) + 1;
                break;
            case kHalt:
                break;
        }
    }

    Tokens tokens_;
    int64_t ip_;
    int64_t rbase_;
};

const IntCodeInterpreter::OpCodeLengths IntCodeInterpreter::kOpLens = {
    {OpCode::kAdd, 3},           {OpCode::kMultiply, 3},
    {OpCode::kInput, 1},         {OpCode::kOutput, 1},
    {OpCode::kJmpIfTrue, 2},     {OpCode::kJmpIfFalse, 2},
    {OpCode::kLessThan, 3},      {OpCode::kEquals, 3},
    {OpCode::kRelBaseOffset, 1}, {OpCode::kHalt, 0},
};

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "error: missing puzzle input file" << std::endl;
        exit(EXIT_FAILURE);
    }

    std::ifstream input(argv[1]);
    std::string line;
    std::getline(input, line);

    auto tokens = ReadTokens(line);
    IntCodeInterpreter interp(tokens);
    interp.Run();

    exit(EXIT_SUCCESS);
}
