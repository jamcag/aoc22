#include <fstream>
#include <iostream>
#include <map>
#include <queue>
#include <sstream>
#include <string>
#include <vector>

struct Valve {
    std::string name;
    std::vector<std::string> neighbours;
    int rate;
    bool open = false;
};

bool operator==(const Valve& lhs, const std::string& rhs) {
    return lhs.name == rhs;
}

int main() {
    std::ifstream ifs{"input"};
    std::string s;

    std::map<std::string, Valve> valves;
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

    // Find benefit over time and cost-to-activate
    // Benefit is rate * (num_steps_left - (distance + 1))
    std::string last = "AA";
    int minutes_remaining = 30;
    int total = 0;

    while (minutes_remaining > 0) {
        std::queue<std::vector<std::string>> frontier;
        frontier.emplace(std::vector<std::string>{last});
        std::map<std::string, int> cost;
        std::map<std::string, int> reward;
        while (!frontier.empty()) {
            const auto path = frontier.front();
            frontier.pop();
            const auto tail_name = path[path.size() - 1];
            const auto tail = valves[tail_name];
            if (cost.contains(tail.name)) {
                continue;
            }

            cost[tail.name] = path.size();
            const auto &valve = valves[tail.name];
            if (tail.open) {
                reward[tail.name] = 0;
            } else {

                reward[tail.name] = (minutes_remaining - (cost[tail.name])) * valve.rate;
            }

            for (const auto &neighbour: valve.neighbours) {
                if (cost.contains(neighbour)) {
                    continue;
                }
                if (std::find(path.begin(), path.end(), neighbour) != path.end()) {
                    continue;
                }
                std::vector new_path = path;
                new_path.emplace_back(neighbour);
                frontier.emplace(new_path);
            }
        }

        int max = -1;
        std::string best = "";
        for (const auto [k, v] : valves) {
            std::cout << v.name << ": cost=" << cost[k] << ", reward=" << reward[k] << "\n";
            if (reward[k] > max) {
                max = reward[k];
                best = k;
            }
        }
        last = best;
        std::cout << "Going to " << best << " for reward " << max << "\n";
        minutes_remaining -= cost[best];
        std::cout << "Time remaining " << minutes_remaining << "\n";
        total += max;
        valves[best].open = true;
        std::cout << "\n";
    }
    std::cout << total << "\n";
}
