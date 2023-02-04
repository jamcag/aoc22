#include <algorithm>
#include <fstream>
#include <iostream>
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
        if (start == "JJ") {
            int dbg = 1;
        }
        if (!cost_cache_.contains(start)) {
            std::map<std::string, int> costs;
            // Find the shortest paths to every non-zero node.
            std::stack<std::vector<std::string>> frontier;
            std::set<std::string> visited;
            frontier.emplace(std::vector<std::string>{start});
            while (!frontier.empty()) {
                auto cur_path = frontier.top();
                auto cur = cur_path.back();
                frontier.pop();
                if (std::find(positive_rate_valves_.begin(), positive_rate_valves_.end(), cur) != positive_rate_valves_.end()) {
                    costs[cur] = cur_path.size();
                    std::cout << "\tstart=" << start << ", end=" << cur << ", cost=" << costs[cur];
                    std::cout << ", path=";
                    for (const auto valve : cur_path) {
                        std::cout << valve;
                    }
                    std::cout << "\n";
                }
                for (const auto neighbour : valves_[cur].neighbours) {
                    if (!visited.contains(neighbour)) {
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

        for (const auto [k, v] : costs) {
            std::cout << "\tk=" << k << ", v=" << v << "\n";
        }
        return costs;
    }
private:
    std::map<std::string, Valve> valves_;
    std::vector<std::string> positive_rate_valves_;
    std::map<std::string, CostToOpen> cost_cache_;
};
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

        std::cout << "valve.name=" << valve.name << ", valve.rate=" << valve.rate << ", valve.neighbors: ";
        for (const auto neighbor : valve.neighbours) {
            std::cout << neighbor << ", ";
        }
        std::cout << "\n";
    }
    std::cout << "\n";
    auto world = World {valves, positive_rate_valves};
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
        std::cout << "cur_state.current=" << cur_state.current << "\n";
        std::cout << "cur_state.total_flow=" << cur_state.total_flow << "\n";
        std::cout << "cur_state.flow_per_minute=" << cur_state.flow_per_minute << "\n";
        std::cout << "cur_state.time_remaining=" << cur_state.time_remaining << "\n";
        std::cout << "path=";
        for (const auto s : path) { std::cout << s.current;}
        std::cout << "\n";

        // Find the path to all closed valves with rate > 0 using BFS.
        CostToOpen costs = world.get_costs_to_open(cur_state);
        if (costs.empty()) {
            std::cout << "\tempty\n";
            std::cout << "\ttotal=std::max(cur_state.total_flow + cur_state.flow_per_minute * cur_state.time_remaining=" << cur_state.total_flow + cur_state.flow_per_minute * cur_state.time_remaining << ", total=" << total << ")=";
            total = std::max(cur_state.total_flow + cur_state.flow_per_minute * cur_state.time_remaining, total);
            std::cout << total << "\n";
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
                std::cout << "\telse";
                std::cout <<" cur_state.flow_per_minute=" << cur_state.flow_per_minute << ", cur_state.time_remaining=" << cur_state.time_remaining << "\n";
                next.total_flow += cur_state.flow_per_minute * cur_state.time_remaining;
            }
            std::cout << "\t\ttotal=std::max(next.total_flow=" << next.total_flow << ", total=" << total << ")=";
            total = std::max(total, next.total_flow);
            std::cout << total << "\n";
            std::cout << "\tnext.current=" << next.current << "\n";
            std::cout << "\t\tnext.time_remaining=" << next.time_remaining << "\n";
            std::cout << "\t\tnext.total_flow=" << next.total_flow << "\n";
            std::cout << "\t\tnext.flow_per_minute=" << next.flow_per_minute << "\n";
        }
        std::cout << "\ttotal=" << total << "\n";
        std::cout << "\tfrontier.size()=" << frontier.size() << "\n";
    }

    std::cout << total << "\n";
}
