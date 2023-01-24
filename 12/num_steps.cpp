#include <array>
#include <fstream>
#include <iostream>
#include <queue>
#include <sstream>
#include <string>
#include <vector>

// S is start at elevation a
// a is lowest
// z is highest
// E is best signal at elevation z
// Find shortest S-E path.
// Can move at most one elevation level higher or lower.

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

    std::array<int, 2> start;
    std::array<int, 2> end;
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

    // Do standard breadth-first search to find a shortest S-E path length.
    std::queue<std::vector<std::array<int, 2>>> frontier;
    frontier.emplace(std::vector<std::array<int, 2>>{start});
    while (frontier.size() > 0) {
        const auto path = frontier.front();
        frontier.pop();
        const auto [row, col] = path.back();

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
            std::cout << path.size() << "\n";
            return EXIT_SUCCESS;
        }

        // Generate neighbours.
        // Up
        if ((row - 1) >= 0) {
            const std::array<int, 2> up = {row - 1, col};
            const int height_delta = lines[row - 1][col] - lines[row][col];
            if ((height_delta == 1 || height_delta == 0) &&
                std::find(path.begin(), path.end(), up) == path.end()) {
                auto up_path = path;
                up_path.emplace_back(up);
                frontier.emplace(up_path);
            }
        }
        // Down
        if ((row + 1) < lines.size()) {
            const std::array<int, 2> down = {row + 1, col};
            const int height_delta = lines[row + 1][col] - lines[row][col];
            if ((height_delta == 1 || height_delta == 0) &&
                std::find(path.begin(), path.end(), down) == path.end()) {
                auto down_path = path;
                down_path.emplace_back(down);
                frontier.emplace(down_path);
            }
        }

        // Left
        if ((col - 1) >= 0) {
            const std::array<int, 2> left = {row, col - 1};
            const int height_delta = lines[row][col - 1] - lines[row][col];
            if ((height_delta == 1 || height_delta == 0) &&
                std::find(path.begin(), path.end(), left) == path.end()) {
                auto left_path = path;
                left_path.emplace_back(left);
                frontier.emplace(left_path);
            }       
        }

        // Right
        if ((col + 1) < lines[0].size()) {
            const std::array<int, 2> right = {row, col + 1};
            const int height_delta = lines[row][col + 1] - lines[row][col];
            if ((height_delta == 1 || height_delta == 0) &&
                std::find(path.begin(), path.end(), right) == path.end()) {
                auto right_path = path;
                right_path.emplace_back(right);
                frontier.emplace(right_path);
            }       
        }
    }
}
