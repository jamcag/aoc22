#include <algorithm>
#include <fstream>
#include <string>
#include <iostream>

int main() {
    std::ifstream ifs("input");
    std::string s;

    int total = 0;
    while (ifs >> s) {
        const auto dash_itr = std::find(s.begin(), s.end(), ',');

        // Get first pair.
        const auto len = std::distance(s.begin(), dash_itr);
        const std::string first_tasks = s.substr(0, len);
        const auto first_comma_itr = std::find(first_tasks.begin(), first_tasks.end(), '-');
        const auto first_len = std::distance(first_tasks.begin(), first_comma_itr);
        const int a = std::stoi(first_tasks.substr(0, first_len));
        const int b = std::stoi(first_tasks.substr(first_len + 1, first_tasks.size()));

        // Get second pair.
        const std::string second_tasks = s.substr(len+1, s.size());
        const auto second_comma_itr = std::find(second_tasks.begin(), second_tasks.end(), '-');
        const auto second_len = std::distance(second_tasks.begin(), second_comma_itr);
        const int x = std::stoi(second_tasks.substr(0, second_len));
        const int y = std::stoi(second_tasks.substr(second_len + 1, second_tasks.size()));

        if (!((b < x) || (y < a))) {
            total += 1;
        }
    }
    std::cout << total << "\n";
}
