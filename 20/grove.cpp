#include <fstream>
#include <iostream>
#include <map>
#include <vector>

using std::vector;

class Mover {
public:
  Mover(vector<int> data) : data_{data} {
    for (size_t i = 0; i < data_.size(); ++i) {
      indices_.emplace_back(i);
    }
  }

  vector<int> render() {
    return data_;
  }
private:
  vector<int> data_;
  vector<size_t> indices_;
};

int main() {
  std::ifstream ifs{"input"};
  int n;
  vector<int> nums;    
  while (ifs >> n) {
    nums.emplace_back(n);
  }

  vector<int> orig = nums;

  for (size_t i = 0; i < orig.size(); ++i) { 
  }
}
