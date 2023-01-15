// Each line is a rucksack.
// Each character is an item.
// First half of line is first compartment, second half is second compartment.

// For each rucksack, find items in both compartments.
// Find sum of their priorities.
#include <algorithm>
#include <fstream>
#include <iostream>
#include <iterator>
#include <set>
#include <string>
#include <vector>

using std::back_inserter;
using std::cout;
using std::ifstream;
using std::set;
using std::string;
using std::vector;

int score(char c) {
    const int char_code = static_cast<int>(c);
    if (char_code >= 97) {
        return char_code - 96;
    }
    return char_code - 38;
}

int main() {
    ifstream ifs("input");
    string s;

    int total = 0;
    int cnt;
    while (ifs >> s) {
        const int length = s.size();
        const int midpoint = length / 2;
        // Find common items.
        set<char> first_component;
        set<char> second_component;
        for (int i = 0; i < length; ++i) {
            if (i < midpoint) {
                first_component.emplace(s.at(i));
            } else {
                second_component.emplace(s.at(i));
            }
        }
        vector<char> common;
        set_intersection(
            first_component.begin(), first_component.end(),
            second_component.begin(), second_component.end(),
            back_inserter(common));
        for (const char item : common) {
            total += score(item);
        }
    }
    cout << total << "\n";
}
