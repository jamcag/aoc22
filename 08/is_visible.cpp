#include <fstream>
#include <iostream>
#include <string>
#include <vector>

int main() {
    std::ifstream ifs{"input"};
    std::vector<std::vector<int>> heights;
    std::string s;

    while (std::getline(ifs, s)) {
        heights.emplace_back();
        std::vector<int>& row = heights[heights.size() - 1];
        for (int i = 0; i < s.size(); ++i) {
            row.emplace_back(std::stoi(s.substr(i, 1)));
        }
    }

    // not visible if any surrounding tree is >=
    int nrows = heights.size();
    int ncols = heights[0].size(); 
    int total = 2 * ncols + 2 * (nrows - 2);
    for (int r = 1; r < (nrows - 1); ++r) {
        for (int c = 1; c < (ncols - 1); ++c) {
            const int this_height = heights[r][c];

            bool visible_left = true;
            for (int other_c = (c - 1); other_c >= 0; --other_c) {
                const int other_height = heights[r][other_c];
                if (other_height >= this_height) {
                    visible_left = false;
                }
            }

            bool visible_right = true;
            for (int other_c = (c + 1); other_c < ncols; ++other_c) {
                const int other_height = heights[r][other_c];
                if (other_height >= this_height) {
                    visible_right = false;
                }
            }

            bool visible_up = true;
            for (int other_r = (r - 1); other_r >= 0; --other_r) {
                const int other_height = heights[other_r][c];
                if (other_height >= this_height) {
                    visible_up = false;
                }
            }

            bool visible_down = true;
            for (int other_r = (r + 1); other_r < nrows; ++other_r) {
                const int other_height = heights[other_r][c];
                if (other_height >= this_height) {
                    visible_down = false;
                }
            }

            const bool visible = visible_left || visible_right || visible_up || visible_down;
            std::cout << "r=" << r << ", c=" << c << ", this_height=" << this_height << ", visible=" << std::boolalpha << visible << "\n";

            if (visible) {
                total += 1;
            }
        }
    }
    std::cout << total << "\n";
}
