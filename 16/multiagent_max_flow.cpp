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

#include "agent.h"
#include "state.h"
#include "valve.h"
#include "world.h"

int const kTimesteps = 30;

using CostToOpen = std::map<std::string, int>;

/* An Agent is at a location, can move to new locations, and open valves.
 * A State describes where agents are, which valves are open, and how much time is left.
 * A Valve is either open or closed.
 * A World contains the environment which we can search.
 */
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
