#include <fstream>
#include <iostream>
#include <string>

int main() {
    std::ifstream ifs{"input"};

    int curr_max = -1;
    std::string s;

    int total = 0;
    while (std::getline(ifs, s)) {
        if (s.empty()) {
            if (total > curr_max) {
                curr_max = total;
            }
            total = 0;
            continue;
        }

        total += std::stoi(s);
    }

    std::cout << curr_max << "\n";
}
