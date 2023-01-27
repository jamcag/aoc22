// Packet data consists of ints and lists.
#include <any>
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <variant>
#include <vector>

// Note: Need to handle digits 0-10.
std::vector<std::any> read_list(const std::string& s) {
    std::vector<std::any> list;
    std::string cur;

    for (int i = 0; i < s.size(); ++i) {
        const char& c = s.at(i);

        if (c == '[') {
            list.emplace_back(read_list(s.substr(i + 1)));
        } else if (c == ']') {
            if (!cur.empty()) {
                list.emplace_back(std::stoi(cur));
            }
            return list;
        } else if (c == ',') {
            if (!cur.empty()) {
                list.emplace_back(std::stoi(cur));
                cur = "";
            }
        } else {
            cur += c;
        }
    }
    return list;
}

enum Result { in_order, process_more, out_of_order };

Result is_in_order(int l, int r) {
    if (l < r) {
        return in_order;
    }
    if (l == r) {
        return process_more;
    }
    return out_of_order;
}

Result is_in_order(const std::vector<std::any>& l, int r) {
    std::vector<std::any> r_as_list{r};
    return is_in_order(l, r_as_list);
}

Result in_order(int l, const std::vector<std::any>& r) {
    std::vector<std::any> l_as_list{l};
    return is_in_order(l_as_list, r);
}

Result is_in_order(const std::vector<std::any>& l, const std::vector<std::any>& r) {
    for (int i = 0; i < l.size(); ++i) {
        Result result;
        try {
            result = is_in_order(std::any_cast<const int>(l[i]), std::any_cast<const int>(r[i]));
        } catch(const std::bad_cast& e) {
            try {
                result = is_in_order(std::any_cast<const std::vector<std::any>&>(l[i]), std::any_cast<int>(r[i]));
            } catch(const std::bad_cast& e) {
                try {
                    result = is_in_order(std::any_cast<int>(l[i]), std::any_cast<const std::vector<std::any>&>(r[i]));
                } catch(const std::bad_cast& e) {
                    try {
                        result = is_in_order(std::any_cast<const std::vector<std::any>&>(l[i]), std::any_cast<const std::vector<std::any>&>(r[i]));
                    } catch (const std::bad_cast& e) {
                        std::cerr << "error: failed conversion\n";
                        throw;
                    }
                }
            }
        }

        if (result != process_more) {
            return result;
        }
    }
    if (l.size() < r.size()) {
        return in_order;
    }
    return out_of_order;
}

int main() {
    std::ifstream ifs{"input"};
    std::string left;
    std::string right;

    int count = 0;
    while (std::getline(ifs, left) && std::getline(ifs, right)) {
        const std::vector<std::any> l_ints = read_list(left.substr(1));
        const std::vector<std::any> r_ints = read_list(right.substr(1));
        const auto res = is_in_order(l_ints, r_ints);

        if (res == in_order) {
            count++;
        } else if (res == process_more) {
            std::cerr << "got process_more in main loop";
        }
        std::string blank;
        std::getline(ifs, blank);
    }

    std::cout << "count=" << count << "\n";
}
