#include <openssl/evp.h>

#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <queue>
#include <string>
#include <unordered_set>

struct MazeLocation {
    std::string path;
    int x;
    int y;

    MazeLocation(const std::string path_, int x_, int y_)
        : path(path_), x(x_), y(y_) {}

    friend bool operator==(const MazeLocation& a, const MazeLocation& b) {
        return (a.x == b.x) && (a.y == b.y);
    }
};

std::string GetMD5Hash(const std::string& secret) {
    const EVP_MD* md = EVP_get_digestbyname("MD5");
    if (!md) {
        std::cerr << "error: unknown message digest" << std::endl;
        return "";
    }

    unsigned char md_value[EVP_MAX_MD_SIZE] = {0};
    unsigned int md_len = 0;
    EVP_MD_CTX* mdctx = EVP_MD_CTX_create();

    EVP_DigestInit_ex(mdctx, md, NULL);
    EVP_DigestUpdate(mdctx, secret.c_str(), secret.size());
    EVP_DigestFinal_ex(mdctx, md_value, &md_len);
    EVP_MD_CTX_destroy(mdctx);

    std::ostringstream os;
    for (int i = 0; i < md_len; ++i) {
        os << std::setfill('0') << std::setw(2) << std::hex
           << static_cast<int>(md_value[i]);
    }
    return os.str();
}

std::vector<MazeLocation> GetNeighbors(const MazeLocation& loc,
                                       const std::string& passcode,
                                       int maze_x_bound, int maze_y_bound) {
    static const std::unordered_set<char> kOpenDoor = {'b', 'c', 'd', 'e', 'f'};
    std::vector<MazeLocation> neighbors;
    std::string hash = GetMD5Hash(passcode + loc.path);

    if (kOpenDoor.count(hash[0]) && ((loc.y - 1) >= 0)) {
        neighbors.emplace_back(loc.path + std::string(1, 'U'), loc.x,
                               loc.y - 1);
    }
    if (kOpenDoor.count(hash[1]) && ((loc.y + 1) < maze_y_bound)) {
        neighbors.emplace_back(loc.path + std::string(1, 'D'), loc.x,
                               loc.y + 1);
    }
    if (kOpenDoor.count(hash[2]) && ((loc.x - 1) >= 0)) {
        neighbors.emplace_back(loc.path + std::string(1, 'L'), loc.x - 1,
                               loc.y);
    }
    if (kOpenDoor.count(hash[3]) && ((loc.x + 1) < maze_x_bound)) {
        neighbors.emplace_back(loc.path + std::string(1, 'R'), loc.x + 1,
                               loc.y);
    }

    return neighbors;
}

int GetLengthOfLongestPathToVault(const std::string& passcode,
                                  int maze_x_bound = 4, int maze_y_bound = 4) {
    MazeLocation start("", 0, 0);
    MazeLocation target("", maze_x_bound - 1, maze_y_bound - 1);

    std::queue<MazeLocation> nodes;
    nodes.push(start);
    int longest_path = 0;
    int path_len = 0;
    while (!nodes.empty()) {
        int num_nodes = nodes.size();
        for (int i = 0; i < num_nodes; ++i) {
            MazeLocation loc = nodes.front();
            nodes.pop();

            if (loc == target) {
                longest_path = std::max(longest_path, path_len);
                continue;
            }

            std::vector<MazeLocation> neighbors =
                GetNeighbors(loc, passcode, maze_x_bound, maze_y_bound);
            for (const MazeLocation& neighbor : neighbors) {
                nodes.push(neighbor);
            }
        }
        path_len++;
    }
    return longest_path;
}

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "error: missing puzzle input file" << std::endl;
        exit(EXIT_FAILURE);
    }

    std::ifstream input(argv[1]);
    std::string passcode;
    std::getline(input, passcode);

    std::cout << "Answer: " << GetLengthOfLongestPathToVault(passcode)
              << std::endl;

    EVP_cleanup();

    exit(EXIT_SUCCESS);
}
