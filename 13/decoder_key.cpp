// Packet data consists of ints and lists.
#include <any>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <tuple>
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


// Below block doesn't compile for using std::find for the divider packets.
//
// /usr/bin/ld: /tmp/ccYn2sOI.o: in function `operator!=(std::any const&, std::any const&)':
// decoder_key.cpp:(.text+0xb3d): undefined reference to `operator==(std::any const&, std::any const&)'
// collect2: error: ld returned 1 exit status

/*
// Forward declaration
bool operator==(const std::any& lhs, const std::any& rhs);

bool operator!=(const std::any& lhs, const std::any& rhs) {
    return !(lhs == rhs);
}
bool operator==(const std::any lhs, const std::any rhs) {
    try {
        return std::any_cast<int>(lhs) == std::any_cast<int>(rhs);
    } catch (const std::bad_cast& e) {
        try {
            const auto lhs_list = std::any_cast<std::vector<std::any>>(lhs);
            const auto rhs_list = std::any_cast<std::vector<std::any>>(rhs);

            if (lhs_list.size() != rhs_list.size()) {
                return false;
            }

            for (int i = 0; i < lhs_list.size(); ++i) {
                if (lhs_list[i] != rhs_list[i]) {
                    return false;
                }
            }
            return true;
        } catch (const std::bad_cast& e) {
            return false;
        }
    }
    return false;
}
*/

int main() {
    std::ifstream ifs{"input"};
    std::string left;
    std::string right;

    int sum_idx = 0;
    int idx = 1;
    std::vector<std::vector<std::any>> packets;
    while (std::getline(ifs, left) && std::getline(ifs, right)) {
        const auto [l_list, _] = read_list(left.substr(1));
        packets.emplace_back(l_list);
        const auto [r_list, __] = read_list(right.substr(1));
        packets.emplace_back(r_list);

        std::string blank;
        std::getline(ifs, blank);
    }

    const auto divider_1 = std::vector<std::any>{std::vector<std::any>{2}};
    packets.emplace_back(divider_1);
    const auto divider_2 = std::vector<std::any>{std::vector<std::any>{6}};
    packets.emplace_back(divider_2);

    auto packet_lt = [](const std::vector<std::any>& lhs, const std::vector<std::any>& rhs) {
        return is_in_order(lhs, rhs) == in_order || is_in_order(lhs, rhs) == process_more;
    };
    std::sort(packets.begin(), packets.end(), packet_lt);

    // Below two lines would be nicer but doesn't compile for some reason since it doesn't like my operator== defined above.
    // const auto idx_divider_1 = std::distance(packets.begin(), std::find(packets.begin(), packets.end(), divider_1));
    // const auto idx_divider_2 = std::distance(packets.begin(), std::find(packets.begin(), packets.end(), divider_2));

    int idx_divider_1{};
    int idx_divider_2{};
    for (int i = 0; i < packets.size(); ++i) {
        std::cout << packets[i] << '\n';
        try {
            const auto packet = std::any_cast<std::vector<std::any>>(packets[i]);
            if (packet.size() != 1) {
                continue;
            }
            const auto inner_packet = std::any_cast<std::vector<std::any>>(packet[0]);
            if (inner_packet.size() == 1 && std::any_cast<int>(inner_packet[0]) == 2) {
                idx_divider_1 = i + 1;
            }
            if (inner_packet.size() == 1 && std::any_cast<int>(inner_packet[0]) == 6) {
                idx_divider_2 = i + 1;
            }
        } catch (const std::bad_cast& e) {
            continue;
        }
    }

    std::cout << "idx_divider_1=" << idx_divider_1 << '\n';
    std::cout << "idx_divider_2=" << idx_divider_2 << '\n';
    // Not 24375
    // Not 24056
    std::cout << "idx_divider_1 * idx_divider_2=" << idx_divider_1 * idx_divider_2 << '\n';
}
