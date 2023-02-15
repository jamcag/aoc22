#include <fstream>
#include <iostream>
#include <map>
#include <queue>
#include <sstream>
#include <string>

using std::map;
using std::string;

struct Expression {
  string operand1 = "";
  string operand2 = "";
  string op = "";
  int64_t value = 0;
};

int64_t calculate_value(string name, const map<string, Expression> monkeys) {
  const auto monkey = monkeys.at(name);
  int64_t ret; 
  if (monkey.op == "") {
    ret = monkey.value;
  } else if (monkey.op == "*") {
    ret = calculate_value(monkey.operand1, monkeys) * calculate_value(monkey.operand2, monkeys);
  } else if (monkey.op == "-") {
    ret = calculate_value(monkey.operand1, monkeys) - calculate_value(monkey.operand2, monkeys);
  } else if (monkey.op == "+") {
    ret = calculate_value(monkey.operand1, monkeys) + calculate_value(monkey.operand2, monkeys);
  } else if (monkey.op == "/") {
    ret = calculate_value(monkey.operand1, monkeys) / calculate_value(monkey.operand2, monkeys);
  } else {
    throw std::exception("bad operand");
  }
  std::cout << "name=" << name << ", monkey.op=" << monkey.op;
  std::cout <<  ", ret=" << ret << "\n";
  return ret;
}

// Each monkey is given a job: either to yell a specific number or to yell the result of a math operation.
int main() {
  std::ifstream ifs{"input"};
  string s;
  map<string, Expression> monkeys;

  while (std::getline(ifs, s)) {
    std::clog << "s=" << s;
    const string monkey_name = s.substr(0, 4);
    std::cout << "monkey_name=" << monkey_name <<"\n";
    const string expr = s.substr(5, s.size());
    
    // Try to read the expression as an int.
    std::istringstream iss{expr};
    int i;
    Expression expression;
    std::cout << "\texpr=" << expr << "\n";
    if (iss >> i) {
      expression.value = i;
      std::clog << "\texpression.value=" << expression.value << "\n";
    } else {
      std::istringstream iss{expr};
      iss >> expression.operand1 >> expression.op >> expression.operand2;
      std::clog << "\t expression.operand1=" << expression.operand1
        << ", expression.op=" << expression.op
        << ", expression.operand2=" << expression.operand2 << "\n";
    }

    monkeys[monkey_name] = expression;
  }

  std::cout << calculate_value("root", monkeys) << "\n";
}