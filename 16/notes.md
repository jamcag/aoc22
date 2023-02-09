# 20230206
## Data Structures
```py
class Agent:
  loc: str
  time_to_dst: int

class State:
  elf: Agent
  eleph: Agent
  time_remaining: int
  open_valves: asd

class World:
  valves: Mapping[str, Valve]
  positive_valves: List[str]

class Valve:
  name: str
  neighbours: List[str]
  rate: int
```
## Algorithm
- Start with the elephant and elf at node "AA"
- Create a queue and enqueue a state with elf and elph both at "AA"
- While the  queue is non-empty:
  - Pop a state $s$ from the queue
  - If elf needs a new vertex, pick one
  - If elph needs a new vertex, pick one
  - If state is terminal then update the

# 20230204
Need to come up with an example where it skips over one.

```
Valve AA has flow rate=0; tunnels lead to valves BB, CC
Valve BB has flow rate=10; tunnels lead to valves AA
Valve CC has flow rate=5; tunnels lead to valves AA, DD
Valve DD has flow rate=20; tunnels lead to valves CC
```


# 20230202
We can compress the graph by removing valves with 0 rate.
For every valve v with rate r > 0 we say u is adjacent to v in V' with edge e' with cost c if there exists a u-v path u, w_1, ..., w_k, v where w_1, ..., w_k have rates 0.
The valve AA is always in G'.

start at AA
while there is still enough time:
    find the path to all closed valves with rate > 0
    go to each one and open that one
    subtract off time from remaining time

```
State:
  current: String
  open: [String]
  time_remaining: int
  total_flow: 0
```
# 20230201
With total time of 9 minutes and input 
```
Valve AA has flow rate=0; tunnels lead to valves BB, CC
Valve BB has flow rate=10; tunnels lead to valves AA
Valve CC has flow rate=0; tunnels lead to valves AA, DD
Valve DD has flow rate=11; tunnels lead to valves CC
```

The optimal moves are:
```
== Minute 1 ==
No valves are open.
You move to valve BB.

== Minute 2 ==
No valves are open.
You open valve BB.

== Minute 3 ==
Valve BB is open, releasing 10 pressure.
You move to valve AA.

== Minute 4 ==
Valve BB is open, releasing 20 pressure.
You move to valve CC.

== Minute 5 ==
Valve BB is open, releasing 30 pressure.
You move to valve DD.

== Minute 6 ==
Valve BB is open, releasing 40 pressure.
You open valve DD.

== Minute 7 ==
Valve BB is open, releasing 61 pressure.
You stay at valve DD.
```

The greedy algorithm which finds the highest time-adjusted reward reachable vertex from the current vertex would go to DD first and open it and then run out of actions as there are no more reachable nodes with rewards.

```
== Minute 1 ==
Calculate the reward of all reachable nodes.
AA: cost=1, reward=0
BB: cost=2, reward=5*10=50
CC: cost=2, reward=0
DD: cost=3, reward=42
Decide to move to DD.
Go to CC.

== Minute 2 == 
Go to DD.

== Minute 3 == 
Open DD.

== Minute 4 ==
Valve DD is open, released 14 pressure.
Stay at DD.

== Minute 5 == 
Valve DD is open, released 28 pressure so far.
Stay at DD.

== Minute 6 == 
Valve DD is open, released 33 pressure so far.
Stay at DD.

== Minute 7 == 
Valve DD is open, released 56 pressure so far.
Stay at DD.
```

We have a gap of 5 between the greedy solution and the optimal solution.

I think with exhaustive search we wouldn't need to store the states.
Instead at each iteration, we can do depth-first search so we're storing at most $O(t)$ nodes.
We then decide which of the actions releases the most pressure.
