#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <iterator>
#include <queue>
#include <sstream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using BuildGraph = std::unordered_map<char, std::unordered_set<char>>;

struct Worker {
  char task = '\0';
  int ticks = 0;
  bool done = false;

  bool IsIdle() const { return (task == '\0'); }
  void Step() {
    if (IsIdle()) {
      return;
    }

    ticks++;
    if (ticks == (60 + (task - 'A' + 1))) {
      done = true;
    }
  }
  void Reset() {
    task = '\0';
    ticks = 0;
    done = false;
  }
};

void AddDependency(BuildGraph& graph, const std::string& text) {
  std::istringstream iss(text);
  std::vector<std::string> words;
  std::copy(std::istream_iterator<std::string>(iss),
            std::istream_iterator<std::string>(), std::back_inserter(words));

  graph[words[7][0]].insert(words[1][0]);
  graph[words[1][0]];
}

std::vector<char> TopologicalSort(BuildGraph& graph) {
  std::priority_queue<char, std::vector<char>, std::greater<char>> choices;
  for (const auto& [node, dependencies] : graph) {
    if (dependencies.empty()) {
      choices.push(node);
    }
  }

  std::vector<char> result;
  std::queue<char> no_dependencies;
  no_dependencies.push(choices.top());
  choices.pop();
  while (!no_dependencies.empty()) {
    char curr = no_dependencies.front();
    no_dependencies.pop();

    result.push_back(curr);

    for (auto& [node, dependencies] : graph) {
      if (dependencies.count(curr)) {
        dependencies.erase(curr);

        if (dependencies.empty()) {
          choices.push(node);
        }
      }
    }

    if (!choices.empty()) {
      no_dependencies.push(choices.top());
      choices.pop();
    }
  }
  return result;
}

bool BuildComplete(const std::vector<char>& build_order) {
  for (const char& c : build_order) {
    if ('.' != c) {
      return false;
    }
  }
  return true;
}

bool AllWorkersIdle(const std::vector<Worker>& workers) {
  for (const Worker& w : workers) {
    if (!w.IsIdle()) {
      return false;
    }
  }
  return true;
}

void AssignTask(Worker& worker, std::vector<char>& build_order,
                BuildGraph& dep_graph) {
  for (char& c : build_order) {
    if ('.' == c) {
      continue;
    }

    if (dep_graph[c].empty()) {
      worker.task = c;
      c = '.';
      break;
    }
  }
}

bool NotAllIdle(const std::vector<Worker>& workers) {
  for (const Worker& w : workers) {
    if (!w.IsIdle()) {
      return true;
    }
  }
  return false;
}

int TotalTimeToBuild(std::vector<char>& build_order, BuildGraph& dep_graph,
                     int num_workers) {
  std::vector<Worker> workers(num_workers);
  int elapsed_time_sec = 0;

  AssignTask(workers[0], build_order, dep_graph);
  while (!BuildComplete(build_order) || NotAllIdle(workers)) {
    for (Worker& w : workers) {
      if (w.IsIdle()) {
        AssignTask(w, build_order, dep_graph);
      }
    }

    for (Worker& w : workers) {
      w.Step();

      if (w.done) {
        // Remove workers task from all dependency sets.
        for (auto& [node, dependencies] : dep_graph) {
          if (dependencies.count(w.task)) {
            dependencies.erase(w.task);
          }
        }
        // Reset the worker so he's ready to be assigned more work.
        w.Reset();
      }
    }
    elapsed_time_sec++;
  }
  return elapsed_time_sec;
}

int main(int argc, char** argv) {
  if (argc != 2) {
    std::cerr << "error: missing puzzle input file" << std::endl;
    exit(EXIT_FAILURE);
  }

  std::ifstream input(argv[1]);
  std::string line;
  BuildGraph g1, g2;
  while (std::getline(input, line)) {
    AddDependency(g1, line);
    AddDependency(g2, line);
  }

  const int kNumWorkers = 5;
  std::vector<char> build_order = TopologicalSort(g1);
  std::cout << "Answer: " << TotalTimeToBuild(build_order, g2, kNumWorkers)
            << std::endl;

  exit(EXIT_SUCCESS);
}
