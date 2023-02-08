#ifndef __WORLD_H__
#define __WORLD_H__

#include <map>
#include <string>
#include <vector>

#include "valve.h"

namespace aoc {

using CostToOpen = std::map<std::string, int>;

class World {
public:
 World(std::map<std::string, Valve> valves,
       std::vector<std::string> positive_rate_valves) :
  valves_{valves},
    positive_rate_valves_{positive_rate_valves} {}

  CostToOpen get_costs_to_open(State state);
  std::vector<State> expand(State state);
  int get_rate(std::string valve) const;
private:
  std::map<std::string, Valve> valves_;
  std::vector<std::string> positive_rate_valves_;
  std::map<std::string, CostToOpen> cost_cache_;
};

} // namespace aoc

#endif // __WORLD_H__
