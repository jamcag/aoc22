#include <array>
#include <cstring>
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>

// Coordinate system: (0, 0) is at top left.
using Coord = std::array<int, 2>;
using Line = std::vector<Coord>;

Coord read_coord(const std::string& s) {
    std::string cur = "";
    for (int i = 0; i < s.size(); ++i) {
        char c = s.at(i);
        if (c == ',') {
            const int x = std::stoi(s.substr(0, i));
            const int y = std::stoi(s.substr(i + 1));
            return {x, y};
        }
    }
    return {-1, -1};
}

int main() {
    std::ifstream ifs{"input"};
    std::string s;

    std::vector<Line> lines;
    while (std::getline(ifs, s)) {
        std::istringstream iss{s};
        std::string x_comma_y;
        lines.emplace_back();
        while (iss >> x_comma_y) {
            const auto coord = read_coord(x_comma_y);
            lines.back().emplace_back(coord);
            const auto [x, y] = coord;
            std::cout << "x=" << x << ", y=" << y << '\n';
            std::string arrow;
            iss >> arrow;
        }
    }

    
}
