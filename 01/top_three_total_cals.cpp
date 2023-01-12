#include <fstream>
#include <iostream>
#include <queue>
#include <string>

int main() {
    std::ifstream ifs{"input"};

    std::priority_queue<int> pq;
    std::string s;

    int total = 0;
    while (std::getline(ifs, s)) {
        if (s.empty()) {
            pq.push(total);
            total = 0;
            continue;
        }

        total += std::stoi(s);
    }

    int top_three_total = 0;
    for (int i = 0; i < 3; ++i) {
        top_three_total += pq.top();
        pq.pop();
    }
    std::cout << top_three_total << "\n";
}
