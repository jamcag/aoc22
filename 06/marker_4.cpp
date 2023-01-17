#include <iostream>
#include <fstream>
#include <queue>
#include <set>

int main() {
    std::ifstream ifs{"input"};
    std::string s;
    ifs >> s;

    for (int i = 3; i < s.size(); ++i) {
        std::set<char> last_four_unique;
        for (int j = i-3; j <= i; ++j) {
            last_four_unique.emplace(s.at(j));
        }
        if (last_four_unique.size() == 4) {
            std::cout << i+1 << "\n";
            return EXIT_SUCCESS;
        }
    }
    return 0;
}
