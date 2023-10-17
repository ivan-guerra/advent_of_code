#include <fstream>
#include <iostream>
#include <string>
#include <unordered_set>
#include <vector>

using SeatTable = std::vector<std::string>;

std::size_t TableHash(const SeatTable& seats) {
  std::string hash_str;
  for (const std::string& row : seats) {
    hash_str += row;
  }
  return std::hash<std::string>{}(hash_str);
}

void UpdateSeats(SeatTable& seats) {
  std::vector<std::pair<int, int>> directions = {
      {0, 1}, {1, 0}, {1, 1}, {-1, 0}, {0, -1}, {-1, -1}, {1, -1}, {-1, 1}};
  int rlim = seats.size();
  int clim = seats[0].size();
  for (int row = 0; row < seats.size(); ++row) {
    for (int col = 0; col < seats[row].size(); col++) {
      int num_occupied = 0;
      for (const auto& dir : directions) {
        int rr = row + dir.first;
        int cc = col + dir.second;
        if ((rr >= 0) && (rr < rlim) && (cc >= 0) && (cc < clim)) {
          num_occupied +=
              (seats[rr][cc] == '#' || seats[rr][cc] == '-') ? 1 : 0;
        }
      }
      // @ -> empty to occupied
      // - -> occupied to empty
      if ((seats[row][col] == 'L') && !num_occupied) {
        seats[row][col] = '@';
      } else if ((seats[row][col] == '#') && (num_occupied >= 4)) {
        seats[row][col] = '-';
      }
    }
  }

  for (auto& row : seats) {
    for (char& seat : row) {
      if (seat == '@') {
        seat = '#';
      } else if (seat == '-') {
        seat = 'L';
      }
    }
  }
}

int CountOccupiedSeats(const SeatTable& seats) {
  int num_occupied = 0;
  for (const auto& row : seats) {
    for (const char& seat : row) {
      if (seat == '#') {
        num_occupied++;
      }
    }
  }
  return num_occupied;
}

int main(int argc, char** argv) {
  if (argc != 2) {
    std::cerr << "error: missing puzzle input file" << std::endl;
    exit(EXIT_FAILURE);
  }

  std::ifstream fhandle(argv[1]);
  std::string line;
  SeatTable seats;
  while (std::getline(fhandle, line)) {
    seats.push_back(line);
  }

  std::unordered_set<std::size_t> history;
  std::size_t seat_hash = TableHash(seats);
  while (!history.count(seat_hash)) {
    history.insert(seat_hash);
    UpdateSeats(seats);
    seat_hash = TableHash(seats);
  }

  std::cout << "Answer: " << CountOccupiedSeats(seats) << std::endl;

  exit(EXIT_SUCCESS);
}
