#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>

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

std::string DecompressFile(const std::string& file) {
    std::string decompressed_file;

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
            std::string comp_text;
            while ((i < file.size()) && comp_marker.char_count--) {
                comp_text += file[i++];
            }

            std::string decompressed_text;
            while (comp_marker.num_repetitions--) {
                decompressed_text += comp_text;
            }

            decompressed_file += decompressed_text;
        } else {
            decompressed_file += file[i++];
        }
    }
    return decompressed_file;
}

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "error: missing puzzle input file" << std::endl;
        exit(EXIT_FAILURE);
    }

    std::ifstream input(argv[1]);
    std::string file;
    std::getline(input, file);

    std::cout << "Answer: " << DecompressFile(file).size() << std::endl;

    exit(EXIT_SUCCESS);
}
