#include <array>
#include <fstream>
#include <iostream>
#include <set>

int main() {
    std::ifstream ifs{"input"};
    char dir;
    int len;

    std::array<int, 2> h;
    std::array<int, 2> t;

    std::set<std::array<int, 2>> visited{};
    visited.emplace(h);
    while (ifs >> dir >> len) {
        for (int i = 0; i < len; ++i) {
            if (dir == 'L') {
                h[0]--;
            } else if (dir == 'U') {
                h[1]++;
            } else if (dir == 'R') {
                h[0]++;
            } else if (dir == 'D') {
                h[1]--;
            }

            const int horizontal_dist = std::abs(h[0] - t[0]);
            const int vertical_dist = std::abs(h[1] - t[1]);
            if (horizontal_dist == 0 && vertical_dist == 0 ||
                horizontal_dist == 0 && vertical_dist == 1 ||
                horizontal_dist == 1 && vertical_dist == 0 ||
                horizontal_dist == 1 && vertical_dist == 1) {
                continue;
            }

            if (horizontal_dist == 2) {
                t[0] = (h[0] + t[0]) / 2;

                if (vertical_dist == 1) {
                    t[1] = h[1];
                }
            }

            if (vertical_dist == 2) {
                t[1] = (h[1] + t[1]) / 2;
                if (horizontal_dist == 1) {
                    t[0] = h[0];
                }
            }
            visited.emplace(t);
        }
    }

    std::cout << visited.size() << "\n";
}
