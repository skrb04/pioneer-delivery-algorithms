# Pioneer Delivery - Part I (Greedy Algorithms)

Dylan Leber - CS 413

Language: C++17

## How to compile and run
    g++ -std=c++17 -O2 -o pioneer main.cpp
    ./pioneer deliveries.txt
    ./pioneer --test

The first run does Tasks A, B and C on a data file. The second runs all 18 required test cases and prints PASS or FAIL for each.

## Input format
Each line is `id start finish`, like `D01 10 13`. Lines starting with `#` get skipped. `deliveries.txt` has the 7 example deliveries from the assignment.

## How the program is set up
- `readDeliveries` reads the file into a vector of deliveries.
- `maxOneDriver` is Task A.
- `assignAllDrivers` is Task B.
- `countRejected` is Task C.
- `printA` and `printB` handle output.
- `runTests` checks all the required cases.

## Task A
1. **Greedy choice:** always take the delivery that finishes earliest, as long as it starts at or after the last one I picked finishes.
2. **Why it's optimal:** say some optimal schedule starts with a different delivery. I can swap it for the earliest-finishing one and nothing overlaps, since the earliest one ends no later. So greedy is never worse, and the same argument works on whatever is left.
3. **Running time:** sorting is O(n log n) and the loop after it is O(n), so O(n log n).

## Task B
1. **Priority queue:** it holds (finish time, driver number) for each driver's last delivery. The driver who frees up soonest is on top.
2. **Picking a driver:** I go through deliveries sorted by start time. If the top driver finishes at or before the new start, that driver takes it. If not, I add a new driver.
3. **Why it's minimum:** I only add a driver when every current driver is still busy at that start time, so that many deliveries overlap at once. You need at least that many drivers, so the count can't be lower.
4. **Running time:** sorting is O(n log n), and n heap operations at O(log n) each is also O(n log n).

## Task C
1. **Link to Task A:** the deliveries I keep are a max compatible set, which is what Task A finds. Everything else gets rejected.
2. **How many rejected:** if the max is k, then n - k have to be rejected.
3. **Running time:** same as Task A, O(n log n).

## Task D
Deliveries:
- D1: R = 0, P = 10
- D2: R = 0, P = 9
- D3: R = 1, P = 8

**Greedy G** goes by highest profit first:
- D1 takes slot 0, so V = 10.
- D2 takes slot 1, delay 1, so V = 9/2 = 4.5.
- D3 is ready at 1 but that slot is taken, so it takes slot 2, delay 1, so V = 8/2 = 4.
- V(G) = 18.5

**Better schedule S:**
- D1 in slot 0, V = 10.
- D3 in slot 1, no delay, V = 8.
- D2 in slot 2, delay 2, V = 9/3 = 3.
- V(S) = 21

Since 21 > 18.5, G isn't always optimal. G puts D2 in slot 1 just because it has more profit than D3, and that blocks D3's only on-time slot. It's cheaper to push D2 back than to push D3 back.

## Test results
All 18 required tests pass (A1-A6, B1-B6, C1-C6). Output of `./pioneer --test`:

    A1: got 0 expected 0 PASS
    A2: got 1 expected 1 PASS
    A3: got 3 expected 3 PASS
    A4: got 1 expected 1 PASS
    A5: got 3 expected 3 PASS
    A6: got 2 expected 2 PASS
    B1: got 0 expected 0 PASS
    B2: got 1 expected 1 PASS
    B3: got 1 expected 1 PASS
    B4: got 3 expected 3 PASS
    B5: got 2 expected 2 PASS
    B6: got 2 expected 2 PASS
    C1: got 0 expected 0 PASS
    C2: got 0 expected 0 PASS
    C3: got 0 expected 0 PASS
    C4: got 2 expected 2 PASS
    C5: got 1 expected 1 PASS
    C6: got 1 expected 1 PASS
