#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <sstream>

using std::cout;
using std::getline;
using std::ifstream;
using std::istringstream;
using std::map;
using std::string;

constexpr int ROCK_SCORE = 1;
constexpr int PAPER_SCORE = 2;
constexpr int SCISSORS_SCORE = 3;
constexpr int LOSS_SCORE = 0;
constexpr int DRAW_SCORE = 3;
constexpr int WIN_SCORE = 6;

int main() {
    // TODO(#1): Why can't we make this const?
    map<char, map<char, int>> scores{
        {'A', {{'X', ROCK_SCORE + DRAW_SCORE}, {'Y', PAPER_SCORE + WIN_SCORE}, {'Z', SCISSORS_SCORE + LOSS_SCORE}}},
        {'B', {{'X', ROCK_SCORE + LOSS_SCORE}, {'Y', PAPER_SCORE + DRAW_SCORE}, {'Z', SCISSORS_SCORE + WIN_SCORE}}},
        {'C', {{'X', ROCK_SCORE + WIN_SCORE}, {'Y', PAPER_SCORE + LOSS_SCORE}, {'Z', SCISSORS_SCORE + DRAW_SCORE}}},
    };

    ifstream ifs("input");
    string s;
    int total = 0;
    while (getline(ifs, s)) {
        istringstream iss(s);
        char opponent;
        iss >> opponent;
        char mine;
        iss >> mine;
        total += scores[opponent][mine];
    }
    cout << total << "\n";
}
