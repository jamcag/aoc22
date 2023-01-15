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
    string s1, s2, s3;

    int total = 0;
    while (ifs >> s1 >> s2 >> s3) {
        set<char> first_component;
        set<char> second_component;
        set<char> third_component;
        for (int i = 0; i < s1.size(); ++i) {
            first_component.emplace(s1.at(i));
        }
        for (int i = 0; i < s2.size(); ++i) {
            second_component.emplace(s2.at(i));
        }
        for (int i = 0; i < s3.size(); ++i) {
            third_component.emplace(s3.at(i));
        }


        vector<char> first_and_second;
        set_intersection(
            first_component.begin(), first_component.end(),
            second_component.begin(), second_component.end(),
            back_inserter(first_and_second));
        vector<char> common;
        set_intersection(
            first_and_second.begin(), first_and_second.end(),
            third_component.begin(), third_component.end(),
            back_inserter(common));

        for (const char item : common) {
            total += score(item);
        }
    }
    cout << total << "\n";
}
