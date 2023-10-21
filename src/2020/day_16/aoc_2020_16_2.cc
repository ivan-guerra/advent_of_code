#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

using Range = std::pair<int, int>;
using FieldMatrix = std::vector<std::vector<int>>;

struct TicketField {
  std::string name;
  Range lower;
  Range upper;

  friend bool operator<(const TicketField& a, const TicketField& b) {
    return (a.name < b.name);
  }
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

void MarkInvalidFields(const std::vector<TicketField>& fields,
                       FieldMatrix& neighbor_values) {
  for (int i = 0; i < neighbor_values.size(); ++i) {
    for (int j = 0; j < neighbor_values[i].size(); ++j) {
      if (!ValidForAtLeastOne(fields, neighbor_values[i][j])) {
        neighbor_values[i][0] = -1; /* invalid ticket entry */
        break;
      }
    }
    if (neighbor_values[i][0] == -1) {
      neighbor_values[i].clear();
    }
  }
}

bool AllRowsFollowOrdering(const std::vector<TicketField>& fields,
                           const FieldMatrix& neighbor_values) {
  for (int i = 0; i < neighbor_values.size(); ++i) {
    if (!neighbor_values[i].empty()) {
      for (int j = 0; j < neighbor_values[i].size(); ++j) {
        if (!InBounds(neighbor_values[i][j], fields[j].lower) &&
            !InBounds(neighbor_values[i][j], fields[j].upper)) {
          return false;
        }
      }
    }
  }
  return true;
}

void FindFieldOrdering(std::vector<TicketField>& fields,
                       const FieldMatrix& neighbor_values) {
  std::sort(std::begin(fields), std::end(fields));
  (void)neighbor_values;
  do {
    if (AllRowsFollowOrdering(fields, neighbor_values)) {
      break;
    }
  } while (std::next_permutation(std::begin(fields), std::end(fields)));
}

std::vector<int> ParseMyTicket(const std::string& infile) {
  std::ifstream fhandle(infile);
  std::string line;

  /* skip field descriptions */
  while (std::getline(fhandle, line) && !line.empty()) {
  }

  std::getline(fhandle, line); /* skip my ticket header */
  std::getline(fhandle, line); /* read my ticket values */

  std::vector<int> values;
  std::stringstream ss(line);
  while (std::getline(ss, line, ',')) {
    values.push_back(std::stoi(line));
  }
  return values;
}

std::vector<int> FindDepartIndices(const std::vector<TicketField>& fields) {
  std::vector<int> indices;
  for (int i = 0; i < fields.size(); ++i) {
    if (fields[i].name.find("departure") == 0) {
      indices.push_back(i);
    }
  }
  return indices;
}

int main(int argc, char** argv) {
  if (argc != 2) {
    std::cerr << "error: missing puzzle input file" << std::endl;
    exit(EXIT_FAILURE);
  }

  /* filter invalid tickets */
  std::ifstream fhandle(argv[1]);
  std::vector<TicketField> fields = ParseFields(fhandle);
  SkipMyTicket(fhandle);
  FieldMatrix nearby_fields = ParseFieldMatrix(fhandle);
  MarkInvalidFields(fields, nearby_fields);

  /* brute force search for the correct ticket ordering */
  FindFieldOrdering(fields, nearby_fields);

  /* multiply the value of all fields start with "departure" */
  std::vector<int> my_ticket = ParseMyTicket(argv[1]);
  std::vector<int> depart_field_indices = FindDepartIndices(fields);
  int result = 1;
  for (const int& i : depart_field_indices) {
    result *= my_ticket[i];
  }

  std::cout << "Answer: " << result << std::endl;

  exit(EXIT_SUCCESS);
}
