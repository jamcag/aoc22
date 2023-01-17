#include <iostream>
#include <fstream>
#include <set>

int main() {
    std::ifstream ifs{"input"};
    std::string s;
    ifs >> s;

    for (int i = 13; i < s.size(); ++i) {
        std::set<char> last_fourteen_unique;
        for (int j = i-13; j <= i; ++j) {
            last_fourteen_unique.emplace(s.at(j));
        }
        if (last_fourteen_unique.size() == 14) {
            std::cout << i+1 << "\n";
            return EXIT_SUCCESS;
        }
    }
    return 0;
}
