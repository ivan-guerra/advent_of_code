#include <cstdlib>
#include <fstream>
#include <iostream>

int SumAllNumbers(const std::string& doc) {
    int sum = 0;
    int i = 0;
    while (i < doc.size()) {
        if (std::isdigit(doc[i])) {
            int j = i;
            while ((i < doc.size()) && std::isdigit(doc[i])) {
                i++;
            }
            sum += std::stoi(doc.substr(j, i - j));
        } else if ('-' == doc[i]) {
            if (((i + 1) < doc.size()) && std::isdigit(doc[i + 1])) {
                i++;
                int j = i;
                while ((i < doc.size()) && std::isdigit(doc[i])) {
                    i++;
                }
                sum += -1 * std::stoi(doc.substr(j, i - j));
            }
        } else {
            i++;
        }
    }
    return sum;
}

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "error: missing puzzle input file" << std::endl;
        exit(EXIT_FAILURE);
    }

    std::ifstream input(argv[1]);
    std::string doc;
    std::getline(input, doc);

    std::cout << "Answer: " << SumAllNumbers(doc) << std::endl;

    exit(EXIT_SUCCESS);
}
