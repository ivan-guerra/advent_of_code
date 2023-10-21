#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using Range = std::pair<int, int>;
using FieldMatrix = std::vector<std::vector<int>>;

struct TicketField {
  std::string name;
  Range lower;
  Range upper;
};

std::vector<TicketField> ParseFields(std::ifstream& fhandle) {
  std::vector<TicketField> fields;
  std::string line;
  while (std::getline(fhandle, line) && !line.empty()) {
    TicketField field;
    int colon_idx = line.find(':');
    field.name = line.substr(0, colon_idx);

    int or_idx = line.rfind("or");
    std::string r1 = line.substr(colon_idx + 2, or_idx - colon_idx - 3);
    int dash1_idx = r1.find('-');
    field.lower.first = std::stoi(r1.substr(0, dash1_idx));
    field.lower.second = std::stoi(r1.substr(dash1_idx + 1));

    std::string r2 = line.substr(or_idx + 3);
    int dash2_idx = r2.find('-');
    field.upper.first = std::stoi(r2.substr(0, dash2_idx));
    field.upper.second = std::stoi(r2.substr(dash2_idx + 1));

    fields.push_back(field);
  }
  return fields;
}

void SkipMyTicket(std::ifstream& fhandle) {
  std::string line;
  while (std::getline(fhandle, line) && !line.empty()) {
  }
}

FieldMatrix ParseFieldMatrix(std::ifstream& fhandle) {
  std::string line;
  std::getline(fhandle, line); /* skip the descriptive text */

  FieldMatrix mat;
  while (std::getline(fhandle, line)) {
    std::string token;
    std::stringstream ss(line);
    std::vector<int> values;
    while (std::getline(ss, token, ',')) {
      values.push_back(std::stoi(token));
    }
    mat.push_back(values);
  }
  return mat;
}

bool InBounds(int value, const Range& range) {
  return (value >= range.first && value <= range.second);
}

bool ValidForAtLeastOne(const std::vector<TicketField>& fields, int value) {
  for (const TicketField& field : fields) {
    if (InBounds(value, field.lower) || InBounds(value, field.upper)) {
      return true;
    }
  }
  return false;
}

int SumOfInvalidFields(const std::vector<TicketField>& fields,
                       const FieldMatrix& neighbor_values) {
  int invalid_sum = 0;
  for (int i = 0; i < neighbor_values.size(); ++i) {
    for (int j = 0; j < neighbor_values[i].size(); ++j) {
      if (!ValidForAtLeastOne(fields, neighbor_values[i][j])) {
        invalid_sum += neighbor_values[i][j];
      }
    }
  }
  return invalid_sum;
}

int main(int argc, char** argv) {
  if (argc != 2) {
    std::cerr << "error: missing puzzle input file" << std::endl;
    exit(EXIT_FAILURE);
  }

  std::ifstream fhandle(argv[1]);

  std::vector<TicketField> fields = ParseFields(fhandle);
  SkipMyTicket(fhandle);
  FieldMatrix nearby_fields = ParseFieldMatrix(fhandle);

  std::cout << "Answer: " << SumOfInvalidFields(fields, nearby_fields)
            << std::endl;

  exit(EXIT_SUCCESS);
}
