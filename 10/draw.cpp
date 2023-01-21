#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

int main() {
    std::ifstream ifs{"input"};
    std::string instruction;

    int x = 1; 
    std::vector<int> x_hist{};
    while (std::getline(ifs, instruction)) {
        std::istringstream iss{instruction};
        
        std::string operation;
        iss >> operation;

        if (operation == "noop") {
            x_hist.emplace_back(x);

        } else if (operation == "addx") {
            int summand;
            iss >> summand;
            x_hist.emplace_back(x);
            x_hist.emplace_back(x);
            x += summand;
        }
    }

    int cycle = 0;
    for (int row = 0; row < 6; ++row) {
        for (int col = 0; col < 40; ++col) {
            if (std::abs(col - x_hist[cycle++]) <= 1) {
                std::cout << "#";
            } else {
                std::cout << ".";
            }
        }
        std::cout << "\n";
    }
}
