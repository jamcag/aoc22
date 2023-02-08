#ifndef __STATE_H__
#define __STATE_H__

#include <string>
#include <vector>

#include "agent.h"
#include "world.h"

namespace aoc {

struct State {
  Agent elf;
  Agent elephant;
  std::vector<std::string> open;
  int time_remaining;
  int flow_per_minute = 0;
  int total_flow = 0;

  std::vector<State> expand(World& world) {
    std::vector<State> states;
    return states;
  }
};

} // namespace aoc

#endif // __STATE_H__
