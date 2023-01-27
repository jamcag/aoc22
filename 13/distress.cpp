// Packet data consists of ints and lists.
#include <any>
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <tuple>
#include <variant>
#include <vector>

// Note: Need to handle digits 0-10.
std::tuple<std::vector<std::any>, int> read_list(const std::string& s) {
    std::vector<std::any> list;
    std::string cur;
    int num_read = 0;
    for (int i = 0; i < s.size(); ++i) {
        const char& c = s.at(i);
        if (c == '[') {
            const auto result = read_list(s.substr(i + 1));
            const auto sub_list = std::get<0>(result);
            const auto len = std::get<1>(result);
            list.emplace_back(sub_list);
            i += len;
        } else if (c == ']') {
            if (!cur.empty()) {
                list.emplace_back(std::stoi(cur));
            }
            return make_tuple(list, i+1);
        } else if (c == ',') {
            if (!cur.empty()) {
                list.emplace_back(std::stoi(cur));
                cur = "";
            }
        } else {
            cur += c;
        }
    }
    return make_tuple(list, s.size());
}

enum Result { in_order, process_more, out_of_order };

// Forward declarations.
Result is_in_order(int l, int r);
Result is_in_order(const std::vector<std::any>& l, int r);
Result is_in_order(int l, const std::vector<std::any>& r);
Result is_in_order(const std::vector<std::any>& l, const std::vector<std::any>& r);

std::ostream& operator<<(std::ostream& os, const std::vector<std::any>& items) {
    std::cout << "[";
    for (const auto item : items) {
        try {
            std::cout << std::any_cast<int>(item) << " ";
        } catch (const std::bad_cast& e) {
            try {
                std::cout << std::any_cast<const std::vector<std::any>>(item) << " ";
            } catch (const std::bad_cast& e) {
                std::cerr << "error: Could not cast\n";
                throw;
            }
        }
    }
    std::cout << "]";
    return os;
}

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

Result is_in_order(int l, const std::vector<std::any>& r) {
    std::vector<std::any> l_as_list{l};
    return is_in_order(l_as_list, r);
}

Result is_in_order(const std::vector<std::any>& l, const std::vector<std::any>& r) {
    // std::cout << "\tl=" << l << std::endl;
    // std::cout << "\tr=" << r << std::endl;
    for (int i = 0; i < l.size(); ++i) {
        if (i == r.size()) {
            return out_of_order;
        }
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
    return process_more;
}

int main() {
    std::ifstream ifs{"input"};
    std::string left;
    std::string right;

    int sum_idx = 0;
    int idx = 1;
    while (std::getline(ifs, left) && std::getline(ifs, right)) {
        std::cout << "left=" << left << "\n";
        std::cout << "right=" << right << "\n";
        const auto [l_ints, l_read] = read_list(left.substr(1));
        const auto [r_ints, r_read] = read_list(right.substr(1));
        const auto res = is_in_order(l_ints, r_ints);

        if (res == in_order) {
            std::cout << "\tidx=" << idx << "\n";
            sum_idx += idx;
        } else if (res == process_more) {
            std::cerr << "\tgot process_more in main loop";
        } else {
            std::cout << "\tnope\n";
        }
        std::string blank;
        std::getline(ifs, blank);
        idx++;
    }

    std::cout << "sum_idx=" << sum_idx << "\n";
}
