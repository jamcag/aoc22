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
    dest_valve: str
    time_to_dest: int

@dataclass
class State:
    elf: Agent
    elph: Agent
    opened: List[str]
    rate: int
    total_flow: int
    time_remaining: int

def expand_state(state: State):
    """Finds the next states that are possible from `state`.
    """
    states = []


    # Case 1: Both need to go somewhere, selet unique ones.
    if state.elf.time_to_dest == 0 and state.elph.time_to_dest == 0:
        elf_costs = cost_to_go[state.elf.dest_valve]
        elph_costs = cost_to_go[state.elph.dest_valve]
        for elf_neighbour, elf_cost in elf_costs.items():
            if elf_neighbour in state.opened:
                continue
            for elph_neighbour, elph_cost in elph_costs.items():
                if elph_neighbour in state.opened:
                    continue
                if elf_neighbour == elph_neighbour:
                    continue

                # Create a state for the closer one.
                # TODO: Will need to figure out what to do when this one is expanded.
                if elf_cost < elph_cost:
                    new_state = State(elf=Agent(elf_neighbour, 0),
                                      elph=Agent(elph_neighbour, elph_cost - elf_cost),
                                      rate = state.rate + rates[elf_neighbour],
                                      total_flow = state.total_flow + state.rate * elf_cost,
                                      time_remaining = state.time_remaining - elf_cost,
                                      opened = state.opened + [elf_neighbour])
                    states.append(new_state)
                elif elph_cost < elf_cost:
                    new_state = State(elf=Agent(elf_neighbour, elph_cost),
                                      elph=Agent(elph_neighbour, elf_cost - elph_cost),
                                      rate = state.rate + rates[elph_neighbour],
                                      total_flow = state.total_flow + state.rate * elph_cost,
                                      time_remaining = state.time_remaining - elph_cost,
                                      opened = state.opened + [elf_neighbour])
                    states.append(new_state)
                else:
                    time_remaining = state.time_remaining - elf_cost

                    if time_remaining < 0:
                        # TODO: Update this
                        new_state = State(current=neighbour,
                                        opened=state.opened + [neighbour],
                                        rate=state.rate + rates[neighbour],
                                        total_flow=state.total_flow + state.rate * cost,
                                        time_remaining=time_remaining)
                        states.append(new_state)
                    else:
                        states.append(State(elf=Agent(elf_neighbour, 0),
                                            elph=Agent(elph_neighbour, 0),
                                            rate=state.rate + rates[elf_neighbour] + rates[elph_neighbour],
                                            total_flow=state.total_flow + state.rate * elf_cost,
                                            time_remaining=state.time_remaining - elf_cost,
                                            opened=state.opened + [elf_neighbour, elph_neighbour]))
    elif state.elf.time_to_dest == 0:
        # Find a new valve for elph to go to.
        for elph_neighbour, elph_cost in cost_to_go[state.elph.dest_valve]:
            elf_cost = state.elf.time_to_dest
            if elf_cost < elph_cost:
                new_state = State(elf=Agent(elf_neighbour, 0),
                                  elph=Agent(elph_neighbour, elph_cost - elf_cost),
                                  rate = state.rate + rates[elf_neighbour],
                                  total_flow = state.total_flow + state.rate * elf_cost,
                                  time_remaining = state.time_remaining - elf_cost,
                                  opened = state.opened + [elf_neighbour])
                states.append(new_state)
            elif elph_cost < elf_cost:
                new_state = State(elf=Agent(elf_neighbour, elph_cost),
                                  elph=Agent(elph_neighbour, elf_cost - elph_cost),
                                  rate = state.rate + rates[elph_neighbour],
                                  total_flow = state.total_flow + state.rate * elph_cost,
                                  time_remaining = state.time_remaining - elph_cost,
                                  opened = state.opened + [elf_neighbour])
                states.append(new_state)
            else:
                time_remaining = state.time_remaining - elf_cost
                states.append(State(elf=Agent(elf_neighbour, 0),
                                elph=Agent(elph_neighbour, 0),
                                rate=state.rate + rates[elf_neighbour] + rates[elph_neighbour],
                                total_flow=state.total_flow + state.rate * elf_cost,
                                time_remaining=state.time_remaining - elf_cost,
                                opened=state.opened + [elf_neighbour, elph_neighbour]))
    return states
expand_state(State(elf=Agent("AA", 0), elph=Agent("AA", 0), opened=[], rate=0, total_flow=0, time_remaining=26))

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

