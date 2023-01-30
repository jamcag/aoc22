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

    std::vector<Coord> impossible;
    for (const auto [sensor, beacon] : sensor_beacon_pairs) {
        const auto [sensor_x, sensor_y] = sensor;
        const auto [beacon_x, beacon_y] = beacon;
        const int sensor_beacon_dist = manhattan_distance(sensor, beacon);

        for (auto cur_y = sensor_y - sensor_beacon_dist; cur_y <= sensor_y + sensor_beacon_dist; ++cur_y) {
            const int remaining_x = sensor_beacon_dist - std::abs(cur_y - sensor_y);
            for (auto cur_x = sensor_x - remaining_x; cur_x <= sensor_x + remaining_x; ++cur_x) {
                const auto current_coord = Coord{cur_x, cur_y};
                if ((cur_y == 10 || cur_y == 2000000) && (current_coord != sensor) && (current_coord != beacon)) {
                    impossible.emplace_back(Coord{cur_x, cur_y});
                }
            }
        }
        std::cout << "Finished (" << sensor_x << ", " << sensor_y << ")\n";
    }

    int cnt_y_eq_10 = 0;
    int cnt_y_eq_2000000 = 0;
    std::sort(impossible.begin(), impossible.end());
    const auto last = std::unique(impossible.begin(), impossible.end());
    impossible.erase(last, impossible.end());
    for (const auto [x, y] : impossible) {
        if (y == 10) {
            cnt_y_eq_10++;
        }
        if (y == 2000000) {
            cnt_y_eq_2000000++;
        }
    }
    std::cout << "cnt_y_eq_10=" << cnt_y_eq_10 << "\n";
    std::cout << "cnt_y_eq_2000000=" << cnt_y_eq_2000000 << "\n";
}
