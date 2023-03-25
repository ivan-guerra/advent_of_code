#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>

int SolveCaptcha(const std::string& captcha) {
    int captcha_sum = 0;
    for (int i = 0; i < captcha.size(); ++i) {
        int neighbor = (i + 1) % captcha.size();
        if (captcha[i] == captcha[neighbor]) {
            captcha_sum += static_cast<int>(captcha[i] - '0');
        }
    }
    return captcha_sum;
}

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "error: missing puzzle input file" << std::endl;
        exit(EXIT_FAILURE);
    }

    std::ifstream input(argv[1]);
    std::string captcha;
    std::getline(input, captcha);

    std::cout << "Answer: " << SolveCaptcha(captcha) << std::endl;

    exit(EXIT_SUCCESS);
}
