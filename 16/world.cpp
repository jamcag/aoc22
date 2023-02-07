#include <map>
#include <queue>
#include <set>
#include <string>
#include <vector>

#include "world.h"

namespace aoc {

CostToOpen World::get_costs_to_open(State state) {
  std::string start = state.current;
  if (!cost_cache_.count(start) > 0) {
    std::map<std::string, int> costs;
    // Find the shortest paths to every non-zero node.
    std::queue<std::vector<std::string>> frontier;
    std::set<std::string> visited;
    frontier.emplace(std::vector<std::string>{start});
    while (!frontier.empty()) {
      auto cur_path = frontier.front();
      auto cur = cur_path.back();
      frontier.pop();
      visited.emplace(cur);
      if (std::find(positive_rate_valves_.begin(), positive_rate_valves_.end(), cur) != positive_rate_valves_.end()) {
        std::cout << "start=" << cur_path[0] << ", end=" << cur << ", cost=" << cur_path.size() << "\n";
        if (cur_path[0] == cur && cur_path.size() != 1) {
          throw;
        }
        costs[cur] = cur_path.size();
      }
      for (const auto neighbour : valves_[cur].neighbours) {
        const bool visited_contains_neighbour = visited.count(neighbour) > 0;
        if (!visited_contains_neighbour) {
          auto new_path = cur_path;
          new_path.emplace_back(neighbour);
          frontier.emplace(new_path);
          visited.emplace(neighbour);
        }
      }
    }
    cost_cache_[start] = costs;
  }
  auto costs = cost_cache_[start];
  for (const auto open_valve : state.open) {
    costs.erase(open_valve);
  }
  return costs;
}

int World::get_rate(std::string valve) const {
  return valves_.at(valve).rate;
}

} // namespace aoc
