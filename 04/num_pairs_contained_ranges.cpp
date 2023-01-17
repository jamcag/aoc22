#include <fstream>
#include <iostream>
#include <ranges>
#include <string>
#include <string_view>
#include <vector>

int main() {
    std::ifstream ifs{"input"};
    std::string s;

    int total = 0;
    while (ifs >> s) {
        const std::string_view sv{s};
        const std::string_view comma_delim{","};
        std::vector<int> endpoints;
        for (const auto interval : std::views::split(sv, comma_delim)) {
            const std::string_view dash_delim{"-"};
            for (const auto endpoint : std::views::split(interval, dash_delim)) {
                const auto endpoint_sv = std::string_view{endpoint.begin(), endpoint.end()};
                const int endpoint_int = std::stoi(endpoint_sv.base());
                endpoints.emplace_back(endpoint_int);
            }
        }

        const int a = endpoints.at(0);
        const int b = endpoints.at(1);
        const int x = endpoints.at(2);
        const int y = endpoints.at(3);

        if ((a <= x && b >= y) || (x <= a && y >= b)) {
            total += 1;
        }
    }

    std::cout << total << "\n";
}