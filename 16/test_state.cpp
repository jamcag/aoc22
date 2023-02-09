#include <string>
#include <vector>

#include "catch2/catch_test_macros.hpp"

#include "agent.h"
#include "state.h"
#include "valve.h"

namespace aoc {

TEST_CASE("States correct when expanding start") {
    std::vector<Valve> valves;
    valves.emplace_back(Valve{"AA", std::vector<std::string>{"BB"}, 0});
    State s;
    Agent elephant;
    Agent elf;
    elf.current = "AA";
    elephant.current = "AA";
}

} // namespace aoc
