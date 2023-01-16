#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <vector>

int main() {
    std::ifstream ifs{"input"};
    std::vector<std::string> crate_stacks;
    std::string s;
    while (std::getline(ifs, s)) {
        if (s.empty()) {
            break;
        }
        crate_stacks.emplace_back(s);
    }
    
    std::istringstream last_line {crate_stacks[crate_stacks.size() - 1]};
    
    int num_stacks{};
    while (last_line >> num_stacks) {}

    // Get the char indices of the stack characters.
    std::map<int, int> stack_to_idx;
    const std::string stack_indices = crate_stacks[crate_stacks.size() - 1];
    for (int i = 0; i < num_stacks; ++i) {
        // The input is 1-indexed while we zero-index.
        stack_to_idx[i] = stack_indices.find(std::to_string(i+1));
    }

    // Fill up the data structures with the items in the stacks.
    std::vector<std::vector<char>> crates;
    for (int i = 0; i < num_stacks; ++i) {
        crates.emplace_back(std::vector<char>{});
    }
    for (int i = crate_stacks.size() - 2; i >= 0; --i) {
        for (const auto& [stack, idx_of_stack] : stack_to_idx) {
            const char stack_char = crate_stacks.at(i).at(idx_of_stack);

            if (stack_char != ' ') {
                crates.at(stack).emplace_back(stack_char);
            }
        }
    }

    // Perform the moves.
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
    }

    for (const auto& crate : crates) {
        std::cout << crate[crate.size() - 1];
    }
    std::cout << "\n";
}