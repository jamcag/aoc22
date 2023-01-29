#include <array>
#include <cstring>
#include <fstream>
#include <iostream>
#include <set>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>

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

    std::set<Coord> rocks;
    // With C++ 23 we could write our for-loop like
    //   for (const auto endpoints : lines | std::views::pairwise) {
    //       const auto [cur, next] = endpoints;
    //   }
    // We don't have a new enough version of the standard library installed.
    for (int i = 0; i < lines.size(); ++i) {
        for (int j = 0; j < (lines.at(i).size() - 1); ++j) {
            const auto [cur_x, cur_y] = lines.at(i).at(j);
            const auto [next_x, next_y] = lines.at(i).at(j + 1);
            rocks.emplace(Coord{cur_x, cur_y});
            rocks.emplace(Coord{next_x, next_y});

            if (cur_x < next_x) {
                const int y = cur_y;
                for (int x = cur_x; x < next_x; ++x) {
                    rocks.emplace(Coord{x, y});
                }
            }

            if (cur_x > next_x) {
                const int y = cur_y;
                for (int x = cur_x; x > next_x; --x) {
                    rocks.emplace(Coord{x, y});
                }
            }

            if (cur_y < next_y) {
                const int x = cur_x;
                for (int y = cur_y; y < next_y; ++y) {
                    rocks.emplace(Coord{x, y});
                }
            }

            if (cur_y > next_y) {
                const int x = cur_x;
                for (int y = cur_y; y > next_y; --y) {
                    rocks.emplace(Coord{x, y});
                }
            }
        }
    }

    // Produce sand one at a time until it comes to rest.
    // Sand will be falling into abyss as soon as y > max(r.y for r in rocks).
    const int max_y = (*std::max_element(rocks.begin(), rocks.end(), [](const auto l, const auto r){ return l[1] < r[1]; }))[1];
    
    std::vector<Coord> occupied;
    std::copy(rocks.begin(), rocks.end(), std::back_inserter(occupied));
    occupied.emplace_back(Coord{500, 0});
    while (true) {
        const auto [sand_x, sand_y] = occupied.back();
        if (std::find(occupied.begin(), occupied.end(), Coord{sand_x, sand_y + 1}) == occupied.end()) {
            occupied.back()[1]++;
            if (occupied.back()[1] == (max_y + 1)) {
                std::cout << "Sand hit ground at (" << sand_x << ", " << sand_y << ")\n";
                occupied.emplace_back(Coord{500, 0});
            }
            continue;
        }
        if (std::find(occupied.begin(), occupied.end(), Coord{sand_x - 1, sand_y + 1}) == occupied.end()) {
            occupied.back()[0]--;
            occupied.back()[1]++;
            if (occupied.back()[1] == (max_y + 1)) {
                std::cout << "Sand hit ground at (" << sand_x << ", " << sand_y << ")\n";
                occupied.emplace_back(Coord{500, 0});
            }
            continue;
        }
        if (std::find(occupied.begin(), occupied.end(), Coord{sand_x + 1, sand_y + 1}) == occupied.end()) {
            occupied.back()[0]++;
            occupied.back()[1]++;
            if (occupied.back()[1] == (max_y + 1)) {
                std::cout << "Sand hit ground at (" << sand_x << ", " << sand_y << ")\n";
                occupied.emplace_back(Coord{500, 0});
            }
            continue;
        }
        if (sand_x == 500 && sand_y == 0) {
            break;
        }
        std::cout << "Sand settled at (" << sand_x << ", " << sand_y << ")\n";
        occupied.emplace_back(Coord{500, 0});
    }

    std::cout << (occupied.size() - rocks.size()) << std::endl;
}
