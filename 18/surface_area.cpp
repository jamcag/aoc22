#include <array>
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <sstream>
#include <vector>

using std::map;
using std::vector;

using Voxel = std::array<int, 3>;

int main() {
    std::ifstream ifs{"input"};
    std::string s;

    vector<Voxel> voxels;
    map<Voxel, int> num_covered;
    while (std::getline(ifs, s)) {
        int x;
        int y;
        int z;
        char comma;
        std::istringstream iss{s};
        iss >> x >> comma >> y >> comma >> z;
        std::cout << "x=" << x << ", y=" << y << ", z=" << z << "\n";
        voxels.emplace_back(Voxel{x, y, z});
        num_covered[Voxel{x, y, z}] = 0;
    }

    for (const auto& voxel : voxels) {
        for (int dim = 0; dim < 3; ++dim) {
            Voxel neighbour = voxel;
            neighbour[dim]--;
            if (num_covered.contains(neighbour)) {
                num_covered[voxel]++;
            }
            neighbour[dim] += 2;
            if (num_covered.contains(neighbour)) {
                num_covered[voxel]++;
            }
        }
    }

    int total_covered = 0;
    for (const auto [_, num] : num_covered) {
        total_covered += num;
    }
    std::cout << (6 * voxels.size()) - total_covered << "\n";
}
