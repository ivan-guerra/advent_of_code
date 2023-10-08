#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

struct Room {
  std::string encrypted_name;
  int id = 0;
  std::string checksum;
};

std::string GetRealRoomName(const Room& room) {
  std::string real_name;
  for (const char& c : room.encrypted_name) {
    if (c == ' ') {
      real_name += ' ';
    } else {
      real_name += ((static_cast<int>(c - 'a') + room.id) % 26) + 'a';
    }
  }
  return real_name;
}

Room ParseRoom(const std::string& room_str) {
  Room room;
  int i = 0;

  /* Parse the encrypted name's character frequencies. */
  while (!std::isdigit(room_str[i])) {
    if ('-' == room_str[i]) {
      room.encrypted_name += ' ';
    } else {
      room.encrypted_name += room_str[i];
    }
    i++;
  }

  /* Trim off the trailing whitespace left by the dash seperating the
     encrypted name from the sector ID. */
  room.encrypted_name =
      room.encrypted_name.substr(0, room.encrypted_name.size() - 1);

  /* Parse sector ID. */
  while (std::isdigit(room_str[i])) {
    room.id = (room.id * 10) + (room_str[i] - '0');
    i++;
  }

  /* Parse checksum. */
  i++;  // Skip opening checksum '['.
  while (room_str[i] != ']') {
    room.checksum += room_str[i];
    i++;
  }
  return room;
}

int main(int argc, char** argv) {
  if (argc != 2) {
    std::cerr << "error: missing puzzle input file" << std::endl;
    exit(EXIT_FAILURE);
  }

  std::ifstream input(argv[1]);
  std::string room;
  std::vector<Room> rooms;
  while (std::getline(input, room)) {
    rooms.push_back(ParseRoom(room));
  }

  const std::string kTargetName = "northpole object storage";
  for (const Room& r : rooms) {
    if (kTargetName == GetRealRoomName(r)) {
      std::cout << "Answer: " << r.id << std::endl;
      exit(EXIT_SUCCESS);
    }
  }

  std::cout << "error: failed to find target name '" << kTargetName << "'"
            << std::endl;
  exit(EXIT_FAILURE);
}
