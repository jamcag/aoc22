#include <algorithm>
#include <fstream>
#include <functional>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

struct Monkey {
    int idx;
    std::vector<int64_t> items;
    std::function<int64_t(int64_t)> op;
    int divisor;
    int true_dst;
    int false_dst;
    int inspections = 0;
};

int main() {
    std::ifstream ifs{"input"};
    std::string s;
    std::vector<Monkey> monkeys;

    while (true) {
        Monkey monkey;
        // Monkey 0: 
        std::string throwaway;
        std::string monkey_line;
        std::getline(ifs, monkey_line);
        std::istringstream monkey_stream{monkey_line};
        monkey_stream >> throwaway;
        monkey_stream >> monkey.idx;
        std::cout << "monkey.idx=" << monkey.idx << "\n";

        //   Starting items: 79, 98
        std::string item_line;
        std::getline(ifs, item_line);
        std::istringstream item_stream{item_line};
        item_stream >> throwaway >> throwaway;
        int item;
        do {
            item_stream >> item;
            monkey.items.emplace_back(item);
        } while (item_stream >> throwaway);
        std::cout << "\tStarting items: ";
        for (const auto item : monkey.items) {
            std::cout << item << ", ";
        }
        std::cout << "\n";

        //  Operation: new = old * 19
        std::string op_line;
        std::getline(ifs, op_line);
        std::istringstream op_stream{op_line};
        for (int i = 0; i < 4; ++i) {
            op_stream >> throwaway;
        }
        char op;
        op_stream >> op;
        std::string operand;
        op_stream >> operand;
        if (op == '-') {
            monkey.op = [operand](int64_t x) { return x - (operand == "old" ? x : std::stoi(operand)); };
        } else if (op == '*') {
            monkey.op = [operand](int64_t x) { return x * (operand == "old" ? x : std::stoi(operand)); };
        } else if (op == '/') {
            monkey.op = [operand](int64_t x) { return x / (operand == "old" ? x : std::stoi(operand)); };
        } else if (op == '+') {
            monkey.op = [operand](int64_t x) { return x + (operand == "old" ? x : std::stoi(operand)); };
        }

        //  Test: divisible by 23
        std::string test_line;
        std::getline(ifs, test_line);
        std::istringstream test_stream{test_line};
        for (int i = 0; i < 3; ++i) {
            test_stream >> throwaway;
        }
        test_stream >> monkey.divisor;

        //     If true: throw to monkey 2
        std::string true_line;
        std::getline(ifs, true_line);
        std::istringstream true_stream{true_line};
        for (int i = 0; i < 5; ++i) {
            true_stream >> throwaway;
        }
        true_stream >> monkey.true_dst;


        //     If false: throw to monkey 3
        std::string false_line;
        std::getline(ifs, false_line);
        std::istringstream false_stream{false_line};
        for (int i = 0; i < 5; ++i) {
            false_stream >> throwaway;
        }
        false_stream >> monkey.false_dst;

        monkeys.emplace_back(monkey);
        if (!(std::getline(ifs, throwaway))) {
            break;
        }
    };

    int prod_of_divisors = 1;
    for (const auto monkey : monkeys) {
        prod_of_divisors *= monkey.divisor;
    }
    for (int round = 0; round < 10000; ++round) {
        std::cout << "Round " << round << "\n";
        for (const auto& cur : monkeys) {
            std::cout << "\tMonkey " << cur.idx << ": ";
            for (const int item : cur.items) {
                std:: cout << item << ", ";
            }
            std::cout << "\n";
        }
        for (int i = 0; i < monkeys.size(); ++i) {
            Monkey& cur = monkeys[i];

            auto items = cur.items;
            auto item_begin = cur.items.begin();
            auto item_end = cur.items.end();
            for (int j = 0; j < items.size(); ++j) {
                cur.inspections++;
                const int64_t worry_level = (cur.op(items[j])) % prod_of_divisors;
                std::cout << "\tmonkey=" << cur.idx << ", item=" << items[j] << ", worry_level=" << worry_level;
                if ((worry_level % cur.divisor) == 0) {
                    std::cout << ", divisible=true, dst=" << cur.true_dst << "\n";
                    monkeys[cur.true_dst].items.emplace_back(worry_level);
                } else {
                    std::cout << ", divisible=false, dst=" << cur.false_dst << "\n";
                    monkeys[cur.false_dst].items.emplace_back(worry_level);
                }
            }

            cur.items.erase(item_begin, item_end);
        }
    }

    std::vector<int64_t> inspections;
    for (const auto monkey : monkeys) {
        std::cout << monkey.inspections << "\n";
        inspections.emplace_back(monkey.inspections);
    }

    std::sort(inspections.begin(), inspections.end(), std::greater<int>());
    std::cout << inspections[0] * inspections[1] << "\n";
}
