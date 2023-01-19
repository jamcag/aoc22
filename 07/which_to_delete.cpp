#include <fstream>
#include <limits>
#include <iostream>
#include <memory>
#include <queue>
#include <sstream>
#include <vector>

struct path {
    path(const std::string name, path* parent, int size, bool is_dir) : name(name), parent(parent), size(size), is_dir(is_dir) {}

    std::string name;
    path* parent;
    int size;
    bool is_dir;
    std::vector<std::unique_ptr<path>> children;
};

int main() {
    std::ifstream ifs{"input"};
    std::string s;

    path root("root", nullptr, 0, true);
    std::vector<path*> dir_history {&root};

    bool ls_mode = false;
    int total = 0;
    while (std::getline(ifs, s)) {
        path* cwd = dir_history[dir_history.size() - 1];
        std::cout << "cwd=" << cwd->name << ", s=" << s << ", ls_mode=" << std::boolalpha << ls_mode <<  "\n";
        if (s.at(0) == '$') {
            ls_mode = false;
            std::istringstream iss{s.substr(2, s.size())};

            std::string cmd;
            iss >> cmd;

            if (cmd == "ls") {
                ls_mode = true;
            } else if (cmd == "cd") {
                std::string dir;
                iss >> dir;
                std::cout << "\tdir=" << dir << "\n";
                if (dir == "..") {
                    dir_history.pop_back();
                } else {
                    for (int i = 0; i < cwd->children.size(); ++i) {
                        path* child = cwd->children[i].get();
                        if (child->name == dir) {
                            dir_history.emplace_back(child);
                            break;
                        }
                    }
                }
            }
        } else {
            std::istringstream iss{s};
            std::string arg;
            iss >> arg;

            if (arg  == "dir") {
                std::string dir_name;
                iss >> dir_name;
                std::unique_ptr<path> child = std::make_unique<path>(dir_name, cwd, 0, true);
                cwd->children.emplace_back(std::move(child));
            } else {
                const int fsize = std::stoi(arg);
                std::string fname;
                iss >> fname;
                for (int i = 0; i < dir_history.size(); ++i) {
                    dir_history[i]->size += fsize;
                }
                std::cout << "fname=" << fname << ", fsize=" << fsize << std::endl;
            }


        }
    }

    std::queue<path*> paths_to_visit;
    paths_to_visit.emplace(&root);

    std::cout << "\n\nTraversing\n";
    int least_size = std::numeric_limits<int>::max();
    const int free_space =  70000000 - root.size;
    const int space_needed = 30000000 - free_space;
    std::cout << "space_needed=" << space_needed << "\n";
    while (paths_to_visit.size() > 0) {
        const path* p = paths_to_visit.front();
        std::cout << "name=" << p->name << ", size=" << p->size << ", least_size=" << least_size << "\n";
        std::cout << std::boolalpha << "\t(p->size > space_needed)=" << (p->size > space_needed) << ", (p->size < least_size)=" << (p->size < least_size) << "\n";
        paths_to_visit.pop();
        if ((p->size > space_needed) && (p->size < least_size)) {
            std::cout << "\tp->size=" << p->size << ", least_size=" << least_size << "\n";
            least_size = p->size;
        }
        for (int i = 0; i < p->children.size(); ++i) {
            path* child = p->children[i].get();
            paths_to_visit.emplace(child);
        }
    }

    std::cout << least_size << "\n";
}
