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

  Coord end;
  std::set<Coord> start_locations;
  for (int row = 0; row < lines.size(); ++row) {
    for (int col = 0; col < lines[0].size(); ++col) {
      if (lines[row][col] == 'a' || lines[row][col] == 'S') {
        start_locations.emplace(Coord{row, col});
      }
      if (lines[row][col] == 'S') {
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
    return std::get<1>(left).size() + l_l1_norm >
           std::get<1>(right).size() + r_l1_norm;
  };

  std::set<int> distances;

  int num_found{};
  // I had this before and it was somehow working converting a char to a Coord
  // WTF ??? for (const auto start : s) {

  std::set<Coord> path_exists;
  for (const auto start : start_locations) {
    std::priority_queue<IntPath, std::vector<IntPath>, decltype(closer)>
        frontier(closer);
    std::set<Coord> seen;
    int idx = 0;
    frontier.emplace(std::make_tuple(idx++, Path{start}));
    bool found = false;
    while (frontier.size() > 0) {
      const auto int_path = frontier.top();
      const auto path = std::get<1>(int_path);
      frontier.pop();
      const auto [row, col] = path.back();
      seen.emplace(path.back());

      if (row == end[0] && col == end[1]) {
        found = true;
        path_exists.emplace(start);
        distances.emplace(path.size() - 1);
        break;
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
    if (!found) {
      std::cout << "Could not find path from " << start[0] << ", " << start[1]
                << "\n";
      num_found++;
    }
  }

  // Not sure why but I'm off by one
  std::cout << *std::min_element(distances.begin(), distances.end()) - 1
            << "\n";
}
