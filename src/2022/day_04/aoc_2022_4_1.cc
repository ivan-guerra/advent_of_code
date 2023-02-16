#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <utility>

using Range = std::pair<int, int>;

std::pair<Range, Range> ParseRanges(const std::string& line) {
    int comma_pos = line.find(',');
    std::string range1_str = line.substr(0, comma_pos);
    std::string range2_str = line.substr(comma_pos + 1, line.size());

    int hyphen1_pos = range1_str.find('-');
    Range range1;
    range1.first = std::stoi(range1_str.substr(0, hyphen1_pos));
    range1.second =
        std::stoi(range1_str.substr(hyphen1_pos + 1, range1_str.size()));

    int hyphen2_pos = range2_str.find('-');
    Range range2;
    range2.first = std::stoi(range2_str.substr(0, hyphen2_pos));
    range2.second =
        std::stoi(range2_str.substr(hyphen2_pos + 1, range2_str.size()));

    return std::make_pair(range1, range2);
}

bool IsCompletelyOverlappingAssignment(const Range& r1, const Range& r2) {
    bool range2_contains_range1 =
        (r1.first >= r2.first) && (r1.second <= r2.second);

    bool range1_contains_range2 =
        (r2.first >= r1.first) && (r2.second <= r1.second);

    return (range1_contains_range2 || range2_contains_range1);
}

int NumFullyOverlappingAssignments(const std::string& input) {
    std::ifstream assignment_if(input);

    int ans = 0;
    std::string line;
    while (std::getline(assignment_if, line)) {
        std::pair<Range, Range> curr_ranges = ParseRanges(line);
        if (IsCompletelyOverlappingAssignment(curr_ranges.first,
                                              curr_ranges.second))
            ans++;
    }
    return ans;
}

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "usage: aoc INPUT_FILE" << std::endl;
        exit(EXIT_FAILURE);
    }

    std::cout << "Number of Complete Overlapping Assignments: "
              << NumFullyOverlappingAssignments(argv[1]) << std::endl;

    exit(EXIT_SUCCESS);
}
