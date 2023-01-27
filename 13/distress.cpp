#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <variant>
#include <vector>

// There should be an Either type or something in the STL.
using Value = std::variant<std::vector<int>, int>;

ostream& operator<<(ostream& os, const Value& v) {

    return os;
}

int main() {
    std::ifstream ifs{"input"};
    std::string left;
    std::string right;

    std::getline(ifs, left);
    std::getline(ifs, right);

    std::istringstream lss{left};
    std::istringstream rss{right};

    std::vector<int> l_ints;
    char left_brack;
    lss >> left_brack;
    char comma;
    int n;
    while (lss >> n) {
        lss >> comma;
        l_ints.emplace_back(n);
    }
    
    std::vector<int> r_ints;
    rss >> left_brack;
    while (rss >> n) {
        rss >> comma;
        r_ints.emplace_back(n);
    }

    std::cout << "l_ints=";
    for (const auto i : l_ints) {
        std::cout << i << " ";
    }

    std::cout << "\nr_ints=";
    for (const auto i : r_ints) {
        std::cout << i << " ";
    }
    std::cout << "\n";
    
    int count = 0;
    // Case 1: Both are integers.
    for (int i = 0; i < l_ints.size(); ++i) {
        if (l_ints[i] == r_ints[i]) {
            continue;
        }
        if (l_ints[i] < r_ints[i]) {
            ++count;
            break;
        }
        if (l_ints[i] > r_ints[i]) {
            break;
        }
    }

    std::cout << "count=" << count << "\n";
}