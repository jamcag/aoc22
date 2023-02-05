#include <algorithm>
#include <deque>
#include <fstream>
#include <iostream>
#include <iterator>
#include <map>
#include <queue>
#include <sstream>
#include <string>
#include <vector>
#include <stack>
#include <set>

int const kTimesteps = 30;

struct Valve {
    std::string name;
    std::vector<std::string> neighbours;
    int rate;
};

struct State {
    std::string current;
    std::vector<std::string> open;
    int time_remaining; 
    int flow_per_minute = 0;
    int total_flow = 0;
};

using CostToOpen = std::map<std::string, int>;
class World {
public:
    World(std::map<std::string, Valve> valves, std::vector<std::string> positive_rate_valves) : valves_{valves}, positive_rate_valves_{positive_rate_valves} {}
    CostToOpen get_costs_to_open(State state) {
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

    int get_rate(std::string valve) const {
        return valves_.at(valve).rate;
    }
private:
    std::map<std::string, Valve> valves_;
    std::vector<std::string> positive_rate_valves_;
    std::map<std::string, CostToOpen> cost_cache_;
};

void valves_to_graphviz(const std::map<std::string, Valve>& valves) {
    std::ofstream ofs{"all_valves.dot"};
    ofs << "graph G {\n";
    for (const auto [k, v] : valves) {
        ofs << "\t" << k << "[label=\"" << k << "\\n" << v.rate << "\"];\n";
    }
    // Print neighbours.
    for (const auto [k, v] : valves) {
        for (const auto neighbour : v.neighbours) {
            ofs << "\t" << k << " -- " << neighbour << ";\n";
        }
    }
    ofs << "}";
}

void important_valves_to_graphviz(const std::vector<std::string>& positive_rate_valves, World& world) {
    std::ofstream ofs{"important_valves.dot"};
    ofs << "graph G {\n";

    std::vector<std::string> important_valves{"AA"};
    std::copy(positive_rate_valves.begin(), positive_rate_valves.end(), std::back_inserter(important_valves));

    for (const auto valve : important_valves) {
        ofs << "\t" << valve << "[label=\"" << valve << "\\n" << world.get_rate(valve) << "\"];\n";
    }

    for (const auto valve : important_valves) {
        State s;
        s.current = valve;
        for (const auto [neighbour, cost] : world.get_costs_to_open(s)) {
            if (valve.at(0) < neighbour.at(0) && valve.at(1) < neighbour.at(1)) {
                ofs << "\t" << valve << " -- " << neighbour << " [label=\"" << cost << "\"];\n";
            }
        }
    }
    ofs << "}";
}

int main() {
    std::ifstream ifs{"input"};
    std::string s;

    std::map<std::string, Valve> valves;
    std::vector<std::string> positive_rate_valves;
    while (std::getline(ifs, s)) {
        std::string _;
        std::string name;
        std::string has;
        std::string flow;
        std::string rate;
        std::string tunnels;
        std::string lead;
        std::string to;
        std::string __;
        std::string other_valve;

        std::istringstream iss{s};
        
        iss >> _ >> name >> has >> flow >> rate;
        valves[name] = Valve{};
        auto& valve = valves[name];
        valve.name = name;
        valve.rate = std::stoi(rate.substr(5, rate.size() - 2));
        if (valve.rate > 0) {
            positive_rate_valves.emplace_back(name);
        }
        iss >> tunnels >> lead >> to >> __;

        while (iss >> other_valve) {
            if (other_valve.size() == 3) {
                other_valve = other_valve.substr(0, 2);
            }
            valve.neighbours.emplace_back(other_valve);
        }
    }
    valves_to_graphviz(valves);
    auto world = World {valves, positive_rate_valves};
    important_valves_to_graphviz(positive_rate_valves, world);
    State initial;
    initial.current = "AA";
    initial.time_remaining = kTimesteps;
    initial.total_flow = 0;

    std::queue<std::vector<State>> frontier;
    frontier.emplace(std::vector<State>{initial});
    int total = 0;
    while (!frontier.empty()) {
        auto path = frontier.front();
        auto cur_state = path.back();

        frontier.pop();
        std::cout << "path=";
        for (const auto s : path) { std::cout << s.current;}
        std::cout << "\n";

        // Find the path to all closed valves with rate > 0 using BFS.
        CostToOpen costs = world.get_costs_to_open(cur_state);
        if (costs.empty()) {
            total = std::max(cur_state.total_flow + cur_state.flow_per_minute * cur_state.time_remaining, total);
        }
        for (const auto [k, v] : costs) {
            State next = cur_state;
            next.current = k;
            next.open = cur_state.open;
            next.open.emplace_back(k);
            next.time_remaining -= (v);

            if (next.time_remaining > 0) {
                next.total_flow += cur_state.flow_per_minute * (v);
                next.flow_per_minute += valves[k].rate;
                auto next_path = path;
                next_path.emplace_back(next);
                frontier.emplace(next_path);

            } else {
                next.total_flow += cur_state.flow_per_minute * cur_state.time_remaining;
            }
            total = std::max(total, next.total_flow);
        }
    }

    std::cout << total << "\n";
}
