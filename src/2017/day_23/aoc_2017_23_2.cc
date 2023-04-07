#include <cmath>
#include <cstdint>
#include <iostream>

int main(int argc, char** argv) {
    (void)argv;
    (void)argc;

    const int64_t start = 93 * 100 + 100000;
    const int64_t end = start + 17000;
    int64_t primes = 0;
    for (int64_t i = start; i != (end + 17); i += 17) {
        bool is_prime = true;
        for (int j = 2; j < std::sqrt(i) + 1; ++j) {
            if (0 == (i % j)) {
                is_prime = false;
                break;
            }
        }

        if (!is_prime) {
            primes++;
        }
    }
    std::cout << "Answer: " << primes << std::endl;

    exit(EXIT_SUCCESS);
}
