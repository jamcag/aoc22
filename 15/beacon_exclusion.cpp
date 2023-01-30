#include <algorithm>
#include <array>
#include <fstream>
#include <iostream>
#include <queue>
#include <set>
#include <sstream>

using Coord = std::array<int, 2>;
using SensorBeacon = std::array<Coord, 2>;

int manhattan_distance(const Coord& x, const Coord& y) {
    return std::abs(x[0] - y[0]) + std::abs(x[1] - y[1]);
}

int main() {
    std::ifstream ifs{"input"};
    std::string s;
    std::vector<SensorBeacon> sensor_beacon_pairs;
    while (std::getline(ifs, s)) {
        // Example line:
        // Sensor at x=2, y=18: closest beacon is at x=-2, y=15
        std::string sensor;
        std::string at;
        std::string sensor_x;
        std::string sensor_y;
        std::string closest;
        std::string beacon;
        std::string is;
        std::string beacon_x;
        std::string beacon_y;

        std::istringstream iss{s};

        iss >> sensor >> at >> sensor_x >> sensor_y >> closest >> beacon >> is >> at >> beacon_x >> beacon_y;

        const int sx = std::stoi(sensor_x.substr(2, sensor_x.size() - 1));
        const int sy = std::stoi(sensor_y.substr(2, sensor_y.size() - 1));
        std::cout << "sx=" << sx << ", sy=" << sy << '\n';

        const int bx = std::stoi(beacon_x.substr(2, beacon_x.size() - 1));
        const int by = std::stoi(beacon_y.substr(2, beacon_y.size() - 1));
        std::cout << "bx=" << bx << ", by=" << by << '\n';
        sensor_beacon_pairs.emplace_back(SensorBeacon{Coord{sx, sy}, Coord{bx, by}});
    }

    std::set<Coord> impossible;

    for (const auto [sensor, beacon] : sensor_beacon_pairs) {
        const auto [sensor_x, sensor_y] = sensor;
        const auto [beacon_x, beacon_y] = beacon;
        const int sensor_beacon_dist = manhattan_distance(sensor, beacon);

        std::queue<Coord> frontier;
        frontier.emplace(sensor);
        std::set<Coord> seen;

        while (!frontier.empty()) {
            const auto [cur_x, cur_y] = frontier.front();
            const int man_dist = manhattan_distance(frontier.front(), sensor);
            if (sensor_x == 0 && sensor_y == 11) {
                std::cout << "sensor_x=" << sensor_x << ", sensor_y=" << sensor_y << ", cur_x=" << cur_x << ", cur_y=" << cur_y << ", sensor_beacon_dist=" << sensor_beacon_dist << ", man_dist=" << man_dist << "\n";
            }
            seen.emplace(frontier.front());
            if (frontier.front() != sensor && frontier.front() != beacon) {
                impossible.emplace(frontier.front());
            }
            frontier.pop();

            const auto up = Coord{cur_x, cur_y - 1};
            if (manhattan_distance(up, sensor) <= sensor_beacon_dist && std::find(seen.begin(), seen.end(), up) == seen.end()) {
                frontier.emplace(up);
            }
            const auto down = Coord{cur_x, cur_y + 1};
            if (manhattan_distance(down, sensor) <= sensor_beacon_dist && std::find(seen.begin(), seen.end(), down) == seen.end()) {
                frontier.emplace(down);
            }
            const auto left = Coord{cur_x - 1, cur_y};
            if (manhattan_distance(left, sensor) <= sensor_beacon_dist && std::find(seen.begin(), seen.end(), left) == seen.end()) {
                frontier.emplace(left);
            }
            const auto right = Coord{cur_x + 1, cur_y};
            if (manhattan_distance(right, sensor) <= sensor_beacon_dist && std::find(seen.begin(), seen.end(), right) == seen.end()) {
                frontier.emplace(right);
            }
        }
        std::cout << "Finished (" << sensor_x << ", " << sensor_y << ")\n";
    }

    int cnt_y_eq_10 = 0;
    int cnt_y_eq_2000000 = 0;
    for (const auto [x, y] : impossible) {
        if (y == 10) {
            std::cout << "x=" << x << ", y=" << y << "\n";
            cnt_y_eq_10++;
        }
        if (y == 2000000) {
            cnt_y_eq_2000000++;
        }
    }
    std::cout << "cnt_y_eq_10=" << cnt_y_eq_10 << "\n";
    std::cout << "cnt_y_eq_2000000=" << cnt_y_eq_2000000 << "\n";
}
