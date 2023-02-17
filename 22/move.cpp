#include <algorithm>
#include <array>
#include <cassert>
#include <functional>
#include <fstream>
#include <iostream>
#include <optional>
#include <set>
#include <string>
#include <unordered_set>
#include <vector>

using std::array;
using std::set;
using std::vector;

using Coord = array<int, 2>;

std::ostream& operator<<(std::ostream& os, const Coord& c) {
  os << "(" << c[0] << ", " << c[1] << ")";
  return os;
}

int pos_mod(int x, int n) {
  return ((x % n) + n) % n;
}

struct World {
  World(const vector<Coord>& free_vec, const vector<Coord>& occupied_vec) {
    std::copy(free_vec.begin(), free_vec.end(), std::inserter(free, std::next(free.begin())));
    std::copy(occupied_vec.begin(), occupied_vec.end(), std::inserter(occupied, std::next(occupied.begin())));

    rows = std::numeric_limits<int>::min();
    cols = std::numeric_limits<int>::min();
    for (const auto& f : free) {
      rows = std::max(f[0], rows);
      cols = std::max(f[1], cols);
    }
    for (const auto& o : occupied) {
      rows = std::max(o[0], rows);
      cols = std::max(o[1], cols);
    }
  }

  bool is_free(const Coord& c) const {
    return free.contains(c);
  }

  bool is_occupied(const Coord& c) const {
    return occupied.contains(c);
  }

  std::function<Coord(const Coord&)> get_forward_fn(char dir) const {
    if (dir == 'R') {
      return [this](const Coord& c) {
        return Coord{c[0], pos_mod(c[1] + 1, cols)};
      };
    } else if (dir == 'L') {
      return [this](const Coord& c) {
        return Coord{c[0], pos_mod(c[1] - 1, cols)};
      };
    } else if (dir == 'U') {
      return [this](const Coord& c) {
        return Coord{pos_mod(c[0] - 1, rows), c[1]};
      };
    } else if (dir == 'D') {
      return [this](const Coord& c) {
        return Coord{pos_mod(c[0] + 1, rows), c[1]};
      };
    }
    throw std::exception("unknown dir");
  }

  Coord move(const Coord& cur, char dir) const {
    bool debug = false;
    if (cur == Coord{35, 50}) {
      debug = true;
    }
    const auto forward = get_forward_fn(dir);
    auto next = forward(cur);
    if (debug) {
      std::cout << "\t\tnext=" << next << "\n";
    }
    while (!is_free(next)) {
      if (is_occupied(next)) {
        return cur;
      }
      next = forward(next);
      if (debug) {
       std::cout << "\t\tnext=" << next << "\n";
      }
    }
    return next;
  }

  char turn(char dir, char turn) const {
    switch(dir) {
      case 'R':
        return turn == 'L' ? 'U' : 'D';
      case 'L':
        return turn == 'L' ? 'D' : 'U';
      case 'U':
        return turn == 'L' ? 'L' : 'R';
      case 'D':
        return turn == 'L' ? 'R' : 'L';
    }
    throw std::exception("could not turn");
  }

  set<Coord> free;
  set<Coord> occupied;
  int rows;
  int cols;
};

int value(char dir) {
  switch(dir) {
    case 'R':
      return 0;
    case 'D':
      return 1;
    case 'L':
      return 2;
    case 'U':
      return 3;
  }
  throw std::exception("bad direction for value");
}
int main() {
  assert(value('R') == 0);
  assert(value('D') == 1);
  assert(value('L') == 2);
  assert(value('U') == 3);
  std::ifstream ifs{"input"};
  std::string s;

  vector<Coord> free;
  vector<Coord> occupied;
  int row = 0;
  std::optional<Coord> first = {};
  while (std::getline(ifs, s)) {
    if (s.empty()) {
      break;
    }
    for (int col = 0; col < s.size(); ++col) {
      if (s.at(col) == '.') {
        if (!first.has_value()) {
          first = Coord{row, col};
        }
        free.emplace_back(Coord{row, col});
      }
      if (s.at(col) == '#') {
        occupied.emplace_back(Coord{row, col});
      }
    }
    ++row;
  }
  const World world = World(free, occupied);
  std::getline(ifs, s);
  std::string cur_dist;
  vector<int> distances;
  vector<char> turns;
  for (int i = 0; i < s.size(); ++i) {
    if (std::isdigit(s.at(i))) {
      cur_dist += s.at(i);
    } else {
      distances.emplace_back(std::stoi(cur_dist));
      turns.emplace_back(s.at(i));
      cur_dist = "";
    }
  }
  std::cout << "cur_dist=" << cur_dist << "\n";
  distances.emplace_back(std::stoi(cur_dist));

  int dist_idx = 0;
  int turn_idx = 0;
  char dir = 'R';
  Coord cur = first.value();
  vector<Coord> curs;
  while (dist_idx < distances.size() || turn_idx < turns.size()) {
    std::cout << "dir=" << dir << ", distances[dist_idx]=" << distances[dist_idx] << "\n";
    for (int step = 0; step < distances[dist_idx]; ++step) {
      cur = world.move(cur, dir);
      // std::cout << "\tcur=" << cur << "\n";
    }
    if (turn_idx < turns.size()) {
      std::cout << "Turning " << turns[turn_idx] << " from " << dir << " to ";
      dir = world.turn(dir, turns[turn_idx++]);
      std::cout << dir << "\n";
    }
    dist_idx++;
    std::cout << "cur=" << cur << "\n";
    curs.emplace_back(cur);
  }
  std::cout << "path = ";
  for (const auto& c : curs) { std::cout << (&c != &curs[0] ? ", " : "") << c; }

  std::cout << 1000 * (cur[0] + 1) + 4 * (cur[1] + 1) + value(dir) << "\n";
}
