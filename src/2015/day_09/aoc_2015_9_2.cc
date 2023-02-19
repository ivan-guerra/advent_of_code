#include <algorithm>
#include <climits>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <iterator>
#include <set>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

class Graph {
   public:
    void AddEdge(const std::string& src, const std::string& dst, int weight);
    int GetCostOfTour(const std::vector<std::string> cities) const;

   private:
    using Edge = std::unordered_map<std::string, int>;
    using AdjList = std::unordered_map<std::string, Edge>;

    AdjList adj_list_;
};

void Graph::AddEdge(const std::string& src, const std::string& dst,
                    int weight) {
    adj_list_[src][dst] = weight;
    adj_list_[dst][src] = weight;
}

int Graph::GetCostOfTour(const std::vector<std::string> cities) const {
    int total_cost = 0;

    for (int i = 0; i < cities.size() - 1; ++i) {
        if (!adj_list_.at(cities[i]).count(cities[i + 1])) {
            return -1;
        }
        total_cost += adj_list_.at(cities[i]).at(cities[i + 1]);
    }
    return total_cost;
}

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "error: missing puzzle input file" << std::endl;
        exit(EXIT_FAILURE);
    }

    std::ifstream input(argv[1]);
    std::string route;
    std::set<std::string> unique_cities;
    Graph routes;
    while (std::getline(input, route)) {
        std::istringstream iss(route);
        std::vector<std::string> tokens;
        std::copy(std::istream_iterator<std::string>(iss),
                  std::istream_iterator<std::string>(),
                  std::back_inserter(tokens));

        unique_cities.insert(tokens[0]);
        unique_cities.insert(tokens[2]);
        routes.AddEdge(tokens[0], tokens[2], std::stoi(tokens[4]));
    }

    std::vector<std::string> cities(unique_cities.begin(), unique_cities.end());
    int max_cost_path = INT_MIN;
    do {
        int cost = routes.GetCostOfTour(cities);
        if (cost != -1) {
            max_cost_path = std::max(max_cost_path, cost);
        }
    } while (std::next_permutation(cities.begin(), cities.end()));

    std::cout << "Answer: " << max_cost_path << std::endl;

    exit(EXIT_SUCCESS);
}
