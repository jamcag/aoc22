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
    std::cout << x_hist[19] * 20 + x_hist[59] * 60 +  x_hist[99] * 100 + x_hist[139] * 140 + x_hist[179] * 180 + x_hist[219] * 220 << "\n";
}
