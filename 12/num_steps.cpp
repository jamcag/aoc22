#include <algorithm>
#include <array>
#include <fstream>
#include <iostream>
#include <queue>
#include <set>
#include <sstream>
#include <string>
#include <tuple>
#include <vector>

// S is start at elevation a
// a is lowest
// z is highest
// E is best signal at elevation z
// Find shortest S-E path.
// Can move at most one elevation level higher or lower.

using Coord = std::array<int, 2>;
using Path = std::vector<Coord>;
using IntPath = std::tuple<int, Path>;

constexpr bool kDebug = false;

int main() {
    std::ifstream ifs{"input"};
    std::string s;

    std::vector<std::vector<char>> lines;
    while (std::getline(ifs, s)) {
        lines.emplace_back();
        std::istringstream iss{s};
        char cell;
        while (iss >> cell) {
            lines.back().emplace_back(cell);
        }
    }

    Coord start;
    Coord end;
    for (int row = 0; row < lines.size(); ++row) {
        for (int col = 0; col < lines[0].size(); ++col) {
            if (lines[row][col] == 'S') {
                start = {row, col};
                lines[row][col] = 'a';
            }
            if (lines[row][col] == 'E') {
                end = {row, col};
                lines[row][col] = 'z';
            }
        }
    }

    auto closer = [end](IntPath left, IntPath right) {
      const auto [l_row, l_col] = std::get<1>(left).back();
      const auto [r_row, r_col] = std::get<1>(right).back();
      const auto l_l1_norm = std::abs(l_row - end[0]) + std::abs(l_col - end[1]);
      const auto r_l1_norm = std::abs(r_row - end[0]) + std::abs(r_col - end[1]);
      // Prefer *lower* cost-to-go.
      return std::get<1>(left).size() + l_l1_norm > std::get<1>(right).size() + r_l1_norm;
    };

    std::priority_queue<IntPath, std::vector<IntPath>, decltype(closer)> frontier(closer);
    // The first time a node is seen should be the shortest path to that node.
    // This isn't the case by default with a std::priority_queue.
    // We'll need to modify our cmp function so that tiebreaking for equal priorities prefers recently inserted ones.
    // This is otherwise known as a stable priority queue.
    std::set<Coord> seen;
    int idx = 0;
    frontier.emplace(std::make_tuple(idx++, Path{start}));
    while (frontier.size() > 0) {
        const auto int_path = frontier.top();
        const auto path = std::get<1>(int_path);
        frontier.pop();
        const auto [row, col] = path.back();
        seen.emplace(path.back());
        std::cout << "height=" << lines[row][col] << ", path.size()=" << path.size() << "\n";
        if (kDebug) {
            std::vector<std::vector<char>> debug_map;
            for (const auto line : lines) {
                debug_map.emplace_back();
                for (const auto ch : line) {
                    debug_map.back().emplace_back(ch);
                }
            }

            for (int i = 1; i < path.size(); ++i) {
                const auto [cur_row, cur_col] = path[i];
                const auto [prev_row, prev_col] = path[i - 1];

                if ((cur_row - prev_row) == 1) {
                    debug_map[prev_row][prev_col] = 'v';
                } else if ((prev_row - cur_row) == 1) {
                    debug_map [prev_row][prev_col] = '^';
                } else if ((cur_col - prev_col) == 1) {
                    debug_map[prev_row][prev_col] = '>';
                } else {
                    debug_map[prev_row][prev_col] = '<';
                }
            }
            debug_map[row][col] = '*';

            for (const auto line : debug_map) {
                for (const auto ch : line) {
                    std::cout << ch;
                }
                std::cout << "\n";
            }

            std::cout << "\n";
        }

        if (row == end[0] && col == end[1]) {
            std::cout << (path.size() - 1) << "\n";
            return EXIT_SUCCESS;
        }

        // Generate neighbours.
        // Up
        if ((row - 1) >= 0) {
            const Coord up = {row - 1, col};
            const int height_delta = lines[row - 1][col] - lines[row][col];
            auto up_path = path;
            up_path.emplace_back(up);

            if ((height_delta <= 1) &&
                std::find(path.begin(), path.end(), up) == path.end() &&
                std::find(seen.begin(), seen.end(), up) == seen.end()) {
                frontier.emplace(std::make_tuple(idx++, up_path));
                seen.emplace(up);
            }
        }
        // Down
        if ((row + 1) < lines.size()) {
            const Coord down = {row + 1, col};
            const int height_delta = lines[row + 1][col] - lines[row][col];
            auto down_path = path;
            down_path.emplace_back(down);

            if ((height_delta <= 1) &&
                std::find(path.begin(), path.end(), down) == path.end() &&
                std::find(seen.begin(), seen.end(), down) == seen.end()) {
                frontier.emplace(std::make_tuple(idx++, down_path));
                seen.emplace(down);
            }
        }

        // Left
        if ((col - 1) >= 0) {
            const Coord left = {row, col - 1};
            const int height_delta = lines[row][col - 1] - lines[row][col];
            auto left_path = path;
            left_path.emplace_back(left);

            if ((height_delta <= 1) &&
                std::find(path.begin(), path.end(), left) == path.end() &&
                std::find(seen.begin(), seen.end(), left) == seen.end()) {
                frontier.emplace(std::make_tuple(idx++, left_path));
                seen.emplace(left);
            }
        }

        // Right
        if ((col + 1) < lines[0].size()) {
            const Coord right = {row, col + 1};
            const int height_delta = lines[row][col + 1] - lines[row][col];
            auto right_path = path;
            right_path.emplace_back(right);

            if ((height_delta <= 1) &&
                std::find(path.begin(), path.end(), right) == path.end() &&
                std::find(seen.begin(), seen.end(), right) == seen.end()) {
                frontier.emplace(std::make_tuple(idx++, right_path));
                seen.emplace(right);
            }
        }
    }
    std::cout << "Exiting\n";
}
