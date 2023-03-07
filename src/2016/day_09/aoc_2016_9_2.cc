#include <cstdint>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

struct CompressionMarker {
    int char_count = 0;
    int num_repetitions = 0;
};

CompressionMarker ParseCompMarker(const std::string& marker) {
    int delim = marker.find('x');
    CompressionMarker comp_marker = {
        .char_count = std::stoi(marker.substr(0, delim)),
        .num_repetitions = std::stoi(marker.substr(delim + 1)),
    };
    return comp_marker;
}

uint64_t DecompressedFileLength(const std::string& file) {
    std::vector<uint64_t> weights(file.size(), 1ULL);
    uint64_t length = 0;
    int i = 0;
    while (i < file.size()) {
        if (file[i] == '(') {
            i++;  // Consume the marker's opening '('.
            std::string marker;
            while ((i < file.size()) && (file[i] != ')')) {
                marker += file[i++];
            }
            i++;  // Consume the marker's closing ')'.

            CompressionMarker comp_marker = ParseCompMarker(marker);
            int k = i;
            for (int j = 0; j < comp_marker.char_count; ++j) {
                weights[k++] *= comp_marker.num_repetitions;
            }
        } else {
            length += weights[i++];
        }
    }
    return length;
}

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "error: missing puzzle input file" << std::endl;
        exit(EXIT_FAILURE);
    }

    std::ifstream input(argv[1]);
    std::string file;
    std::getline(input, file);

    std::cout << "Answer: " << DecompressedFileLength(file) << std::endl;

    exit(EXIT_SUCCESS);
}
