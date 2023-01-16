#include <fstream>
#include <iostream>
#include <stack>
#include <vector>

int main() {
    std::ifstream ifs{"transformed_input"};
    std::string s;
    std::vector<std::vector<char>> crates;
    while (std::getline(ifs, s)) {
        if (s.empty()) {
            break;
        }
        crates.emplace_back(std::vector<char>{});
        for (int i = 0; i < s.size(); ++i) {
            crates[crates.size() - 1].emplace_back(s.at(i));
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
        const int start_idx = crates.at(src - 1).size() - cnt;
        for (int i = 0; i < cnt; ++i) {
            const int elem = crates.at(src - 1)[start_idx + i];
            crates.at(dst - 1).emplace_back(elem);
        }
        for (int i = 0; i < cnt; ++i) {
            crates.at(src - 1).pop_back();
        }
        std::cout << move << " " << cnt << " " << from << " " << src << " " << to << " " << dst << "\n";
        for (const auto& crate : crates) {
            if (crate.size() > 0) {
                std::cout << crate[crate.size() - 1];
            } else {
                std::cout << ".";
            }
        }
        std::cout << "\n\n";
    }

    for (const auto& crate : crates) {
        std::cout << crate[crate.size() - 1] << "\n";
    }
}