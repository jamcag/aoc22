#include <array>
#include <fstream>
#include <iostream>
#include <limits>
#include <map>
#include <string>
#include <sstream>
#include <vector>

using std::map;
using std::numeric_limits;
using std::vector;

using Voxel = std::array<int, 3>;

class Component {
public:

    void add_voxel(const Voxel& voxel) {
        voxels_.emplace_back(voxel);
    }
private:
    vector<Voxel> voxels_;
    int min_x_ = numeric_limits<int>::max();
    int max_x_ = numeric_limits<int>::min();
    int min_y_ = numeric_limits<int>::max();
    int max_y_ = numeric_limits<int>::min();
    int min_z_ = numeric_limits<int>::max();
    int max_z_ = numeric_limits<int>::min();
};

int main() {
    std::ifstream ifs{"input"};
    std::string s;

    vector<Voxel> lava;
    map<Voxel, int> num_covered;
    while (std::getline(ifs, s)) {
        int x;
        int y;
        int z;
        char comma;
        std::istringstream iss{s};
        iss >> x >> comma >> y >> comma >> z;
        std::cout << "x=" << x << ", y=" << y << ", z=" << z << "\n";
        lava.emplace_back(Voxel{x, y, z});
        num_covered[Voxel{x, y, z}] = 0;
    }

    map<Voxel, int> air;
    for (const auto& voxel : lava) {
        for (int dim = 0; dim < 3; ++dim) {
            Voxel neighbour = voxel;
            neighbour[dim]--;
            if (num_covered.contains(neighbour)) {
                num_covered[voxel]++;
            } else if (!air.contains(neighbour)) {
                air[neighbour] = 1;
            } else {
                air[neighbour]++;
            }

            neighbour[dim] += 2;
            if (num_covered.contains(neighbour)) {
                num_covered[voxel]++;
            } else if (!air.contains(neighbour)) {
                air[neighbour] = 1;
            } else {
                air[neighbour]++;
            }
        }
    }

    int total_covered = 0;
    for (const auto [_, num] : num_covered) {
        total_covered += num;
    }

    // Find connected components.
    // Find external surface area of each component.
    // TODO: Account for larger air pockets.
    //       Right now we only voxels as an air pocket if it is covered by lava on all sides.
    //       However a large air pocket, for example a 3x3 one will have a central air pocket
    //       with no neighbouring lava.
    for (const auto [vox, num] : air) {
        const auto [x, y, z] = vox;
        std::cout << "x=" << x << ", y=" << y << ", z=" << z << "\n";
        if (num == 6) {
            total_covered += 6;
        }
    }
    std::cout << (6 * lava.size()) - total_covered << "\n";
}
