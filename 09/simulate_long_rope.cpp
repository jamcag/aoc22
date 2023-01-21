
#include <array>
#include <fstream>
#include <iostream>
#include <set>

int main() {
    std::ifstream ifs{"input"};
    char dir;
    int len;

    std::array<std::array<int, 2>, 10> knots{};
    for (auto& knot : knots) {
        knot[0] = 0;
        knot[1] = 0;
    }

    std::set<std::array<int, 2>> visited{};
    visited.emplace(knots.back());
    while (ifs >> dir >> len) {
        auto& h = knots[0];
        auto& t = knots.back();

        // Step the head len step.
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

            // Step each knot.
            for (int j = 1; j < knots.size(); ++j) {
                auto& prev = knots[j-1];
                auto& cur = knots[j];
                const int horizontal_dist = std::abs(prev[0] - cur[0]);
                const int vertical_dist = std::abs(prev[1] - cur[1]);

                if (horizontal_dist == 0 && vertical_dist == 0 ||
                    horizontal_dist == 0 && vertical_dist == 1 ||
                    horizontal_dist == 1 && vertical_dist == 0 ||
                    horizontal_dist == 1 && vertical_dist == 1) {
                    continue;
                }

                if (horizontal_dist == 2) {
                    cur[0] = (prev[0] + cur[0]) / 2;

                    if (vertical_dist == 1) {
                        cur[1] = prev[1];
                    }
                }

                if (vertical_dist == 2) {
                    cur[1] = (prev[1] + cur[1]) / 2;
                    if (horizontal_dist == 1) {
                        cur[0] = prev[0];
                    }
                }
            }

            visited.emplace(knots.back());
        }
    }

    std::cout << visited.size() << "\n";
}
