#include <fstream>
#include <iostream>
#include <limits>
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
    int max_scenic_score = std::numeric_limits<int>::min();
    for (int r = 0; r < (nrows - 1); ++r) {
        for (int c = 1; c < (ncols - 1); ++c) {
            const int this_height = heights[r][c];

            int scenic_score_left = 0;
            for (int other_c = (c - 1); other_c >= 0; --other_c) {
                scenic_score_left += 1;
                const int other_height = heights[r][other_c];
                if (other_height >= this_height) {
                    break;
                }
            }

            int scenic_score_right = 0;
            for (int other_c = (c + 1); other_c < ncols; ++other_c) {
                scenic_score_right += 1;
                const int other_height = heights[r][other_c];
                if (other_height >= this_height) {
                    break;
                }
                
            }

            int scenic_score_up = 0;
            for (int other_r = (r - 1); other_r >= 0; --other_r) {
                scenic_score_up += 1;
                const int other_height = heights[other_r][c];
                if (other_height >= this_height) {
                    break;
                }
            }

            int scenic_score_down = 0;
            for (int other_r = (r + 1); other_r < nrows; ++other_r) {
                scenic_score_down += 1;
                const int other_height = heights[other_r][c];
                if (other_height >= this_height) {
                    break;
                }
            }

            const int scenic_score = scenic_score_left * scenic_score_right * scenic_score_up * scenic_score_down;

            std::cout << "r=" << r << ", c=" << c << ", score=" << scenic_score << "\n";
            std::cout << "\tright=" << scenic_score_right << ", left=" << scenic_score_left << ", up=" << scenic_score_up << ", down=" << scenic_score_down << "\n";
            if (scenic_score > max_scenic_score) {
                max_scenic_score = scenic_score;
            }
        }
    }
    std::cout << max_scenic_score << "\n";
}
