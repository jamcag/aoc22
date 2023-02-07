#ifndef __VALVE_H__
#define __VALVE_H__

#include <string>
#include <vector>

namespace aoc {

struct Valve {
  std::string name;
  std::vector<std::string> neighbours;
  int rate;
};

} // namespace aoc

#endif // __VALVE_H__
