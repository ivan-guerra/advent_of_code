#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <iterator>
#include <memory>
#include <queue>
#include <sstream>
#include <string>
#include <vector>

struct FileSysNode;
using FileSysNodePtr = std::shared_ptr<FileSysNode>;

struct FileSysNode {
    std::string label;
    int size;
    bool is_dir;
    FileSysNodePtr parent;
    std::vector<FileSysNodePtr> children;

    FileSysNode(const std::string& label_, int size_, bool is_dir_,
                FileSysNodePtr parent_)
        : label(label_), size(size_), is_dir(is_dir_), parent(parent_) {}
};

std::vector<std::string> SplitOnSpace(const std::string& s) {
    std::istringstream buffer(s);
    return std::vector<std::string>(
        (std::istream_iterator<std::string>(buffer)),
        std::istream_iterator<std::string>());
}

std::queue<std::string> LoadSessionData(const std::string& input) {
    std::ifstream session_data(input);
    std::queue<std::string> data;

    std::string line;
    while (std::getline(session_data, line)) {
        data.push(line);
    }
    return data;
}

bool IsCommand(const std::string& line) {
    static const char kCmdStartMarker = '$';
    return (!line.empty() && (line.front() == kCmdStartMarker));
}

FileSysNodePtr ExecChangeDir(const std::vector<std::string>& cmd_tokens,
                             FileSysNodePtr root) {
    std::string target_dir = cmd_tokens[2];

    if (target_dir == "..") {
        // Do nothing if you are already at the root of the filesystem.
        if (root->label == "/") return root;

        // Walk back a single directory.
        return root->parent;
    }

    if (target_dir == "/") {
        // Walk your way 'up' to the root of the file tree.
        while (root->label != "/") root = root->parent;
        return root;
    }

    for (const FileSysNodePtr& child : root->children) {
        // Move down into one of the children.
        if ((child->label == target_dir) && (child->is_dir)) return child;
    }

    // An invalid target directory was given to cd. Just return root.
    std::cerr << "invalid directory: " << target_dir << std::endl;
    return root;
}

void ExecListDir(std::queue<std::string>& session_data, FileSysNodePtr root) {
    while (!session_data.empty() && (!IsCommand(session_data.front()))) {
        std::vector<std::string> ls_tokens = SplitOnSpace(session_data.front());
        if (ls_tokens.front() == "dir") {
            root->children.push_back(
                std::make_shared<FileSysNode>(ls_tokens[1], 0, true, root));
        } else {
            root->children.push_back(std::make_shared<FileSysNode>(
                ls_tokens[1], std::stoi(ls_tokens[0]), false, root));
        }
        session_data.pop();
    }
}

void InitFileTreeSizes(FileSysNodePtr root) {
    if (!root->is_dir) return;

    for (FileSysNodePtr& child : root->children) {
        if (child->is_dir) InitFileTreeSizes(child);

        root->size += child->size;
    }
}

FileSysNodePtr ConstructFileSysTree(const std::string& input) {
    std::queue<std::string> session_data = LoadSessionData(input);

    static const std::string kChangeDir = "cd";
    static const std::string kListDir = "ls";
    FileSysNodePtr root = std::make_shared<FileSysNode>("/", 0, true, nullptr);
    while (!session_data.empty()) {
        if (IsCommand(session_data.front())) {
            std::vector<std::string> cmd_tokens =
                SplitOnSpace(session_data.front());
            if (cmd_tokens[1] == kChangeDir) {
                root = ExecChangeDir(cmd_tokens, root);
                session_data.pop();
            } else if (cmd_tokens[1] == kListDir) {
                session_data.pop();  // Pop off the ls command itself.
                ExecListDir(session_data, root);  // Process the ls output.
            } else {
                std::cerr << "unknown command: " << session_data.front()
                          << std::endl;
            }
        }
    }

    // Get to the root of the tree.
    FileSysNodePtr curr = root;
    while (curr->parent) curr = curr->parent;
    root = curr;

    // Set the size of each node in the file tree.
    InitFileTreeSizes(root);

    return root;
}

int FindSizeOfSmallestDirGeqLimit(FileSysNodePtr root, int limit) {
    std::queue<FileSysNodePtr> nodes;
    nodes.push(root);

    int min_size = root->size;
    while (!nodes.empty()) {
        int level_size = nodes.size();
        for (int i = 0; i < level_size; ++i) {
            FileSysNodePtr curr = nodes.front();
            nodes.pop();

            if (curr->size >= limit) min_size = std::min(min_size, curr->size);

            for (FileSysNodePtr& child : curr->children) {
                if (child->is_dir) nodes.push(child);
            }
        }
    }
    return min_size;
}

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "usage: aoc INPUT_FILE" << std::endl;
        exit(EXIT_FAILURE);
    }

    FileSysNodePtr root = ConstructFileSysTree(argv[1]);
    const int kMaxDiskSpace = 70000000;
    const int kUpdateReqSpace = 30000000;
    int free_space = kMaxDiskSpace - root->size;
    int required_space = kUpdateReqSpace - free_space;

    std::cout << "Size of Directory to Delete: "
              << FindSizeOfSmallestDirGeqLimit(root, required_space)
              << std::endl;

    exit(EXIT_SUCCESS);
}
