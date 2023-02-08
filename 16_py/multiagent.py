from collections import deque
from dataclasses import dataclass
import re
from typing import List

rates = {}
neighbours = {}
with open("input") as f:
    for line in f.readlines():
        match = re.match(r"Valve ([A-Z][A-Z]) has flow rate=(\d+); tunnels? leads? to valves? (.*)", line)
        valve = match[1]
        rate = int(match[2])
        tunnels = [t.strip() for t in match[3].split(",")]
        rates[valve] = rate
        neighbours[valve] = tunnels

# Find shortest paths. cost_to_go[x][y] is the cost to open y from x.
def find_costs(src: str):
    """Finds costs to open positive-rate valves from `src`.
    """
    costs = {}
    frontier = deque()
    frontier.append([src])
    visited = set()

    while len(frontier):
        cur_path = frontier.popleft()
        cur = cur_path[-1]
        for n in neighbours[cur]:
            if rates[cur] > 0:
                costs[n] = len(cur_path)
            if n not in visited:
                visited.add(n)
                frontier.append(cur_path + [n])
    return costs

cost_to_go = {}
for valve, rate in rates.items():
    if rate > 0:
        cost_to_go[valve] = find_costs(valve)

# Do a depth-first search for the highest max.
@dataclass
class Agent:
    valve: str
    time_remaining: int

    def needs_next_valve(self)::
        return time_remaining == 0

@dataclass
class State:
    elf: Agent
    elephant: Agent
    opened: List[str]
    rate: int
    total_flow: int
    time_remaining: int

def expand_state(state: State):
    """Finds the next states that are possible from `state`.
    """
    states = []


    # Case 1: Both need to go somewhere, selet unique ones.
    if state.elf and state.elephant:
        elf_costs = cost_to_go(state.elf)
        eleph_costs = cost_to_go(state.elephant)
        for elf_neighbour, elf_cost in elf_costs.items():
            if elf_neighbour in state.opened:
                continue
            for eleph_neighbour, eleph_cost in eleph_costs.items():
                if eleph.neighbour in state.opened:
                    continue
                if elf_neighbour == eleph_neighbour:
                    continue

                # Create a state for the closer one.
                # TODO: Will need to figure out what to do when this one is expanded.
                if elf_cost < eleph_cost:
                    ...
                elif eleph_cost < elf_cost:
                    ...
                else:
                    time_remaining = state.time_remaining - cost

                    if time_remaining < 0:
                        new_state = State(current=neighbour,
                                        opened=state.opened + [neighbour],
                                        rate=state.rate + rates[neighbour],
                                        total_flow=state.total_flow + state.rate * cost,
                                        time_remaining=time_remaining)
                        states.append(new_state)
                    else:
                        new_state
                        states.append(State(current=state.current,
                                            opened=state))
                ...
initial = State(elf="AA", elephant="AA", opened=[], rate=0, total_flow=0, time_remaining=30)

for idx, state in expand_state(initial):
    print(f"{idx=}, {state=}")

frontier = deque()
frontier.append(initial)
greatest = 0
while len(frontier):
    state = frontier.pop()
    if state.time_remaining > 0:
        expand_state(state).map(frontier.append)
    else:
        greatest = max(greatest, state.total_flow)
print(f"{greatest=}")

