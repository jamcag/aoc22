#ifndef __STATE_H__
#define __STATE_H__

#include <string>
#include <vector>

namespace aoc {

struct State {
  Agent elf;
  Agent elephant;
  std::vector<std::string> open;
  int time_remaining;
  int flow_per_minute = 0;
  int total_flow = 0;
};

} // namespace aoc

#endif // __STATE_H__
