#include <algorithm>
#include <array>
#include <fstream>
#include <iostream>
#include <limits>
#include <string>
#include <vector>

using std::array;
using std::vector;

using Coord = array<int, 2>;

constexpr bool kDebug = false;

bool all_unique(const vector<Coord>& v1, const vector<Coord>& v2) {
    for (const auto& coord1 : v1) {
        for (const auto& coord2 : v2) {
            if (false) {
                std::cout << "this_coord.x=" << coord1[0] << ", this_coord.y=" << coord1[1] 
                            << ", other_coord.x=" << coord2[0] << ", other_coord.y="
                            << coord2[1] << "\n";
            }
            if (coord1[0] == coord2[0] && coord1[1] == coord2[1]) {

                return false;
            }
        }
    }
    return true;
}

// Coordinate system has (0, 0) at bottom left.
// Every shape has their own origin.
// Users maintain the transform tree.
class Shape {
public:
    Shape() {}
    Shape(vector<Coord> coords) : coords_{coords} {
        if (coords.empty()) {
            return;
        }
    }
    
    bool collide;

    void set_transform(const Coord& transform) {
        transform_ = transform;
    }

    vector<Coord> get_coords_in_world_frame() const {
        vector<Coord> ret;
        for (auto [x, y] : coords_) {
            ret.emplace_back(Coord{x + transform_[0], y + transform_[1]});
        }
        return ret;
    }

    // Check if this collides with other shapes.
    bool collides(const vector<Shape>& shapes) {
        const auto this_coords = get_coords_in_world_frame();
        for (const auto shape : shapes) {
            const auto other_coords = shape.get_coords_in_world_frame();
            if (!all_unique(this_coords, other_coords)) {
                return true;
            }
        }
        return false;
    }

    // Moves the block down by 1 unit if possible and return true. Returns false if not possible.
    bool move_down_if_possible(const vector<Shape>& shapes) {
        auto shape_down = *this;
        shape_down.transform_[1]--;

        if (shape_down.transform_[1] < 0) {
            return false;
        }

        if (shape_down.collides(shapes)) {
            return false;
        }

        transform_[1]--;
        return true;
    }

    void move_left_if_possible(const vector<Shape>& shapes) {        
        int min_x = std::numeric_limits<int>::max();
        for (auto [x, _] : get_coords_in_world_frame()) {
            min_x = std::min(min_x, x);
        }

        if (min_x > 0) {
            auto shape_left = *this;
            shape_left.transform_[0]--;
            if (!shape_left.collides(shapes)) {
                transform_[0]--;
            }
        }
    }

    void move_right_if_possible(const vector<Shape>& shapes) {
        int max_x = std::numeric_limits<int>::min();
        for (auto [x, _] : get_coords_in_world_frame()) {
            max_x = std::max(max_x, x);
        }

        if (max_x < 6) {
            auto shape_right = *this;
            shape_right.transform_[0]++;
            if (!shape_right.collides(shapes)) {
                transform_[0]++;
            }
        }
    }
private:
    vector<Coord> coords_;
    Coord transform_;
    int height_;
};

std::ostream& operator<<(std::ostream& os, const Shape& s) {
    auto coords = s.get_coords_in_world_frame();
    for (int i = 0; i < coords.size(); ++i) {
        os << "(" << coords.at(i)[0] << ", " << coords.at(i)[1] << ")";
        if (i != (coords.size() - 1)) {
            os << ", ";
        }
    }
    os << "\n";
    return os;
}

Shape make_horizontal() {
    return Shape(vector{Coord{0, 0}, Coord{1, 0}, Coord{2, 0}, Coord{3, 0}});
}
Shape make_plus() {
    return Shape(vector{Coord{0, 1}, Coord{1, 0}, Coord{1, 1}, Coord{1, 2}, Coord{2, 1}});
}
Shape make_l() {
    return Shape(vector{Coord{0, 0}, Coord{1, 0}, Coord{2, 0}, Coord{2, 1}, Coord{2, 2}});
}
Shape make_vertical() {
    return Shape(vector{Coord{0, 0}, Coord{0, 1}, Coord{0, 2}, Coord{0, 3}});
}
Shape make_square() {
    return Shape(vector{Coord{0, 0}, Coord{0, 1}, Coord{1, 0}, Coord{1, 1}});
}

class ShapeGenerator {
public:
    Shape get_next_shape() {
        Shape ret;
        if ((num_generated_ % 5) == 0) { ret = make_horizontal(); }
        if ((num_generated_ % 5) == 1) { ret = make_plus(); }
        if ((num_generated_ % 5) == 2) { ret = make_l(); }
        if ((num_generated_ % 5) == 3) { ret = make_vertical(); }
        if ((num_generated_ % 5) == 4) { ret = make_square(); }
        num_generated_++;
        return ret;
    }
private:
    int num_generated_ = 0;
};

// bool operator==(const Coord& l, const Coord& r) {
//     return l[0] == r[0] && l[1] == r[1];
// }

void render(const Shape& cur, const vector<Shape>& shapes, int spawn_height, int cnt) {
    vector<Coord> moving;
    for (const auto& c : cur.get_coords_in_world_frame()) {
        moving.emplace_back(c);
    }

    vector<Coord> stopped;
    for (const auto& s : shapes) {
        for (const auto& c : s.get_coords_in_world_frame()) {
            stopped.emplace_back(c);
        }
    }

    std::ofstream ofs{std::to_string(cnt) + ".ppm"};
    ofs << "P3\n7" << " " << spawn_height << "\n255\n";
    for (int y = spawn_height - 1; y >= 0; --y) {
        for (int x = 0; x < 7; ++x) {
            if (std::find(moving.begin(), moving.end(), Coord{x, y}) != moving.end()) {
                ofs << "255 0 0\n";
            } else if (std::find(stopped.begin(), stopped.end(), Coord{x, y}) != stopped.end()) {
                ofs << "255 255 255\n";
            }
            else {
                ofs << "0 0 0\n";
            }
        }
    }
}
auto main() -> int {
    std::ifstream ifs{"input"};
    vector<Shape> s;
    ShapeGenerator gen;
    const int spawn_width = 2;
    int spawn_height = 3;

    Shape cur = gen.get_next_shape();
    cur.set_transform(Coord{spawn_width, spawn_height});
    char c;
    if (kDebug) { std::cout << "cur=" << cur; }
    vector<char> pattern;
    while (ifs >> c) {
        pattern.emplace_back(c);
        if (c == '<') {
            cur.move_left_if_possible(s);
        } else if (c == '>') {
            cur.move_right_if_possible(s);
        } else {
            std::cerr << "Unexpected input";
            return EXIT_FAILURE;
        }
        if (kDebug) { std::cout << "c=" << c << ", cur=" << cur; }
        if (!cur.move_down_if_possible(s)) {
            std::cout << "comes to rest\n";
            auto coords = cur.get_coords_in_world_frame();
            for (auto [_, y] : coords) {
                spawn_height = std::max(spawn_height, y + 4);
            }
            s.emplace_back(cur);
            if (kDebug) { std::cout << "s.size()=" << s.size() << ", cur=" << cur << "\n"; }
            cur = gen.get_next_shape();
            cur.set_transform(Coord{spawn_width, spawn_height});
            if (kDebug) { std::cout << "new cur=" << cur; }
        } else {
            std::cout << "down, cur=" << cur;
        }
        if (s.size() == 2022) {
            break;
        }
    }
    int cnt = 0;
    while (s.size() < 2022) {
        c = pattern.at(cnt);
        cnt = (cnt + 1) % pattern.size();
        if (c == '<') {
            cur.move_left_if_possible(s);
        } else if (c == '>') {
            cur.move_right_if_possible(s);
        } else {
            std::cerr << "Unexpected input";
            return EXIT_FAILURE;
        }
        if (kDebug) { std::cout << "c=" << c << ", cur=" << cur; }
        if (!cur.move_down_if_possible(s)) {
            std::cout << "comes to rest\n";
            auto coords = cur.get_coords_in_world_frame();
            for (auto [_, y] : coords) {
                spawn_height = std::max(spawn_height, y + 4);
            }
            s.emplace_back(cur);
            if (kDebug) { std::cout << "s.size()=" << s.size() << ", cur=" << cur << "\n"; }
            cur = gen.get_next_shape();
            cur.set_transform(Coord{spawn_width, spawn_height});
            if (kDebug) { std::cout << "new cur=" << cur; }
        } else {
            std::cout << "down, cur=" << cur;
        }
        if (s.size() == 2022) {
            break;
        }
    }
    std::cout << spawn_height - 3 << "\n";
}
