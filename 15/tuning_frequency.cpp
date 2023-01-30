#include <algorithm>
#include <array>
#include <fstream>
#include <iostream>
#include <queue>
#include <set>
#include <sstream>

using Coord = std::array<int, 2>;

int manhattan_distance(const Coord& x, const Coord& y) {
    return std::abs(x[0] - y[0]) + std::abs(x[1] - y[1]);
}

class Circle {
public:
    Circle(const Coord& center, const int radius) : center{center}, radius{radius} {}
    bool contains(const Coord& x) {
        return manhattan_distance(x, center) <= radius;
    }
private:
    Coord center;
    int radius;
};

int main() {
    std::ifstream ifs{"input"};
    std::string s;
    std::vector<Circle> circles;
    std::array<int, 4000000> n_occupied;
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
        const auto sensor_coord = Coord{sx, sy};
        std::cout << "sx=" << sx << ", sy=" << sy << '\n';

        const int bx = std::stoi(beacon_x.substr(2, beacon_x.size() - 1));
        const int by = std::stoi(beacon_y.substr(2, beacon_y.size() - 1));
        const auto beacon_coord = Coord{bx, by};

        std::cout << "bx=" << bx << ", by=" << by << '\n';
        circles.emplace_back(Circle{sensor_coord, manhattan_distance(sensor_coord, beacon_coord)});
    }


    for (auto x = 0; x < 4000000; ++x) {
        // std::cout << "x=" << x << "\n";
        for (auto y = 0; y < 4000000; ++y) {
            const auto location = Coord{x, y};
            if (std::none_of(circles.begin(), circles.end(), [&location](auto c){ return c.contains(location); })) {
                std::cout << "x=" << x << ", y=" << y << "\n";
                std::cout << 4000000 * x + y << "\n";
            }
        }
    }
}
