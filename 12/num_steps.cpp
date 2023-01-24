#include <algorithm>
#include <array>
#include <fstream>
#include <iostream>
#include <queue>
#include <set>
#include <sstream>
#include <string>
#include <vector>

// S is start at elevation a
// a is lowest
// z is highest
// E is best signal at elevation z
// Find shortest S-E path.
// Can move at most one elevation level higher or lower.

using Coord = std::array<int, 2>;
using Path = std::vector<Coord>;

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

    auto closer_to_end = [&end](Path left, Path right) {
        const Coord left_cur = left.back();
        const Coord right_cur = right.back();
        const auto l = ((left_cur[0] - end[0]) * (left_cur[0] * end[0])) + ((left_cur[1] - end[1]) * (left_cur[1] - end[1]));
        const auto r = ((right_cur[0] - end[0]) * (right_cur[0] * end[0])) + ((right_cur[1] - end[1]) * (right_cur[1] - end[1]));
        return l < r;
    };

    // Prefer climbing.
    auto higher = [lines](Path left, Path right) {
        const auto [l_row, l_col] = left.back();
        const auto [r_row, r_col] = right.back();
        const int l = lines[l_row][l_col];
        const int r = lines[r_row][r_col];
        return l < r;
    };

    auto higher_neighbours = [lines](Path left, Path right) {
        const auto [l_row, l_col] = left.back();
        int l = lines[l_row][l_col];
        if ((l_row - 1) >= 0) {
            l = std::max(l, static_cast<int>(lines[l_row - 1][l_col]));
        }
        if ((l_row + 1) < lines.size()) {
            l = std::max(l, static_cast<int>(lines[l_row + 1][l_col]));
        }
        if ((l_col - 1) >= 0) {
            l = std::max(l, static_cast<int>(lines[l_row][l_col - 1]));
        }
        if ((l_col + 1) < lines[0].size()) {
            l = std::max(l, static_cast<int>(lines[l_row][l_col + 1]));
        }

        const auto [r_row, r_col] = right.back();
        int r = lines[r_row][r_col];
        if ((r_row - 1) >= 0) {
            l = std::max(l, static_cast<int>(lines[r_row - 1][r_col]));
        }
        if ((r_row + 1) < lines.size()) {
            l = std::max(l, static_cast<int>(lines[r_row + 1][r_col]));
        }
        if ((r_col - 1) >= 0) {
            l = std::max(l, static_cast<int>(lines[r_row][r_col - 1]));
        }
        if ((r_col + 1) < lines[0].size()) {
            l = std::max(l, static_cast<int>(lines[r_row][r_col + 1]));
        }
        return l < r;
    };
    std::priority_queue<Path, std::vector<Path>, decltype(higher_neighbours)> frontier(higher_neighbours);
    std::set<Path> seen;
    frontier.emplace(Path{start});
    while (frontier.size() > 0) {
        const auto path = frontier.top();
        frontier.pop();
        const auto [row, col] = path.back();
        seen.emplace(path);
        // const auto cost = ((row - end[0]) * (row - end[0])) + ((col - end[1]) * (col - end[1]));
        std::cout << "cost=" << lines[row][col] << "path.size()=" << path.size() << "\n";
        if (kDebug) {
            std::vector<std::vector<char>> debug_map;
            for (const auto line : lines) {
                debug_map.emplace_back();
                for (const auto ch : line) {
                    debug_map.back().emplace_back('.');
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

            if ((height_delta == 1 || height_delta == 0) &&
                std::find(path.begin(), path.end(), up) == path.end() &&
                std::find(seen.begin(), seen.end(), up_path) == seen.end()) {
                frontier.emplace(up_path);
            }
        }
        // Down
        if ((row + 1) < lines.size()) {
            const Coord down = {row + 1, col};
            const int height_delta = lines[row + 1][col] - lines[row][col];
            auto down_path = path;
            down_path.emplace_back(down);

            if ((height_delta == 1 || height_delta == 0) &&
                std::find(path.begin(), path.end(), down) == path.end() &&
                std::find(seen.begin(), seen.end(), down_path) == seen.end()) {
                frontier.emplace(down_path);
            }
        }

        // Left
        if ((col - 1) >= 0) {
            const Coord left = {row, col - 1};
            const int height_delta = lines[row][col - 1] - lines[row][col];
            auto left_path = path;
            left_path.emplace_back(left);

            if ((height_delta == 1 || height_delta == 0) &&
                std::find(path.begin(), path.end(), left) == path.end() &&
                std::find(seen.begin(), seen.end(), left_path) == seen.end()) {
                frontier.emplace(left_path);
            }       
        }

        // Right
        if ((col + 1) < lines[0].size()) {
            const Coord right = {row, col + 1};
            const int height_delta = lines[row][col + 1] - lines[row][col];
            auto right_path = path;
            right_path.emplace_back(right);

            if ((height_delta == 1 || height_delta == 0) &&
                std::find(path.begin(), path.end(), right) == path.end() &&
                std::find(seen.begin(), seen.end(), right_path) == seen.end()) {
                frontier.emplace(right_path);
            }
        }
    }
}
