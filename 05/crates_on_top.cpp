#include <fstream>
#include <iostream>
#include <stack>
#include <vector>

int main() {
    std::ifstream ifs{"transformed_input"};
    std::string s;
    std::vector<std::stack<char>> crates;
    while (std::getline(ifs, s)) {
        if (s.empty()) {
            break;
        }
        crates.emplace_back(std::stack<char>{});
        for (int i = 0; i < s.size(); ++i) {
            crates[crates.size() - 1].emplace(s.at(i));
        }
    }

    std::cout << "s: (" << s << ")\n";
    
    std::string move;
    int cnt;
    std::string from;
    int src;
    std::string to;
    int dst;

    while (ifs >> move >> cnt >> from >> src >> to >> dst) {
        for (int i = 0; i < cnt; ++i) {
            const int elem = crates.at(src - 1).top();
            crates.at(dst - 1).emplace(elem);
            crates.at(src - 1).pop();
        }
        std::cout << move << " " << cnt << " " << from << " " << src << " " << to << " " << dst << "\n";
        for (const auto& crate : crates) {
            if (crate.size() > 0) {
                std::cout << crate.top();
            } else {
                std::cout << ".";
            }
        }
        std::cout << "\n\n";
    }

    for (const auto& crate : crates) {
        std::cout << crate.top() << "\n";
    }
}