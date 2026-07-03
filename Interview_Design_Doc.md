# Rubik's Cube Solver - Architectural Design & Interview Guide

This document tracks all engineering decisions made during the development of our Rubik's Cube Solver. It is designed to act as your "cheat sheet" for defending your design choices in High-Frequency Trading (HFT) and Big Tech interviews.

## 1. Core Architecture (OOP Design)

**Decision:** We created a `GenericRubiksCube` abstract base class with pure virtual functions (`virtual void move() = 0`, etc.).
* **Alternatives Considered:** Building a single concrete class right away.
* **Why we chose this:** In large systems, you often want to plug in different implementations to benchmark them against each other without changing the search algorithms (e.g., BFS/DFS). By using an abstract base class, our solvers can accept *any* cube implementation that fulfills the contract. This demonstrates a solid grasp of **Polymorphism** and the **Dependency Inversion Principle (SOLID)**.
* **Interview Talking Point:** "I designed the solver engine to be decoupled from the state representation. This allowed me to easily hot-swap a 1D Array implementation with a Bitboard implementation later for benchmarking, without rewriting any of the pathfinding logic."

## 2. State Representation

**Decision:** We map the cube to a flat 1D array: `char cube[54];`.
* **Alternative 1 (3D Array - `char cube[6][3][3]`):** 
  * *Pros:* Very easy to visualize and write manually. 
  * *Cons:* Poor spatial locality. Multi-dimensional arrays can sometimes incur pointer-chasing overhead or complex row-major math under the hood, leading to L1 Cache misses during billions of operations.
* **Alternative 2 (Bitboards - `uint64_t`):**
  * *Pros:* Absolute maximum performance. Uses bitwise operations.
  * *Cons:* Extremely complex to implement and debug.
* **Why we chose 1D Array:** It offers a fantastic balance. `char cube[54]` is contiguous in memory. The entire state is 54 bytes, easily fitting into a single modern CPU cache line (which is typically 64 bytes). This guarantees zero cache misses when accessing the cube state. 
* **Interview Talking Point:** "I opted for a 1D array of 54 characters. Because a modern CPU cache line is 64 bytes, my entire cube state fits perfectly into L1 cache. This spatial locality is critical for performance when expanding millions of nodes per second in IDA*."

## 3. Move Generation (State Transitions)

**Decision:** We hardcoded the 90-degree moves as explicit `O(1)` index swaps based on a pre-computed mathematical map, rather than writing a generic "rotate" algorithm.
* **Alternatives Considered:** Writing a generic 3D rotation matrix function that calculates the new position of each sticker on the fly.
* **Why we chose this:** Calculating indices at runtime (e.g., using modulo arithmetic or matrix multiplication) requires CPU cycles. For an engine evaluating millions of nodes per second, state transitions must be as fast as possible. Hardcoding the 4-cycles does 20 direct memory assignments and nothing else.
* **Interview Talking Point:** "To guarantee correctness, I wrote a Python script to model the 3D Cartesian coordinates and linear algebra rotations. I then exported those generated transformations as hardcoded O(1) assignments in C++. This moved the heavy mathematical lifting to compile-time/generation-time, leaving the runtime loop completely branchless and calculation-free."

## 4. Handling Primes (') and Double (2) Moves

**Decision:** We implemented `Prime` and `Double` moves by calling the basic 90-degree clockwise turn multiple times (e.g., `U'` is `U(); U(); U();`).
* **Alternatives Considered:** Hardcoding all 18 moves explicitly.
* **Why we chose this (for now):** Maintainability. Hardcoding 6 moves is 120 lines of code. Hardcoding 18 is 360 lines. For the first iteration, composing the basic moves reduces the surface area for bugs. If profiling later shows that `U(); U(); U();` is a bottleneck, we can explicitly generate the reverse swaps. 
* **Interview Talking Point:** "I prioritized 'Make it work, make it right, make it fast.' By composing the prime moves from the base moves, I guaranteed correctness first. Since the state is small, a good compiler may even unroll the repeated functions. If I were deploying this to a production latency-critical system, I would explicitly unroll the remaining 12 moves."

## 5. Custom Hashing for Graph Traversal

**Decision:** We built a custom `Hash1d` struct to hash the `char cube[54]` directly, rather than hashing a dynamically allocated string representation.
* **Alternatives Considered:** Using `cube.getString()` and `std::hash<std::string>()`.
* **Why we chose this:** Using `getString()` instantiates a `std::string` on the heap, copying 54 bytes into dynamically allocated memory. In BFS, where we explore hundreds of thousands of nodes per second, the heap allocator becomes a massive bottleneck and leads to memory fragmentation. By writing a direct polynomial rolling hash (`hash = hash * 31 + cube[i]`) over the fixed array, we maintain `O(1)` memory overhead and stay exclusively in the CPU cache.
* **Interview Talking Point:** "When integrating the cube state with `std::unordered_map` for cycle detection in BFS, I wrote a custom hash struct instead of relying on stringification. Heap allocating strings in a hot loop destroys latency. My custom hash iterates over the 54-byte array directly, generating the bucket index with zero allocations, which is a critical design pattern in low-latency HFT environments."

## 6. Graph Search & Path Reconstruction (BFS)

**Decision:** During BFS, we track the shortest path backwards using a simple `std::unordered_map<Cube, MOVE>` rather than storing `std::vector<MOVE>` inside every node in the queue.
* **Alternatives Considered:** Storing a copy of the path vector inside each node pushed to the BFS queue.
* **Why we chose this:** In a Rubik's cube, the branching factor is 18. At depth 6, the queue might hold over 30 million states. If each state also holds a `std::vector` of size 6, the memory overhead destroys the cache and exhausts system RAM rapidly. By storing a 1-byte `MOVE` per visited node, we can perfectly reconstruct the path by inverting the move to find the parent state, tracing all the way back to the root.
* **Interview Talking Point:** "When implementing the BFS solver, the naive approach is to carry the `std::vector` path payload on every state. This is catastrophic for memory. Instead, my `visited` map only stores the 1-byte Move that generated the state. When I find the target, I simply apply the inverse of the move `cube.invert(move)` to deterministically step backward to the parent state, completely eliminating the path-vector memory overhead."

## 7. Memory vs. Optimality (IDDFS) & Branching Factor Pruning

**Decision:** We transitioned our core solver engine from BFS to Iterative Deepening DFS (IDDFS). Furthermore, we implemented integer-division pruning (`(int)move / 3`) to aggressively prune the search tree.
* **Why we chose this:** BFS guarantees the optimal solution but crashes due to memory limits around depth 7. DFS uses $O(D)$ zero-memory overhead, but returns wildly sub-optimal solutions. IDDFS combines the best of both. It loops DFS from depth 1 to $N$. While it theoretically repeats work at the top of the tree, the overwhelming majority of nodes in an exponential tree ($b=18$) exist at the bottom leaf layer, making the repetition overhead mathematically insignificant (< 5%).
* **Pruning Logic:** To prevent useless cycles (e.g., `U` followed by `U'`), we group our 18 moves logically by face. Since integer division rounds down, `0/3`, `1/3`, and `2/3` all equal `0` (the Left face). If the next move targets the same face as the previous move, we `continue` (skip it). This shrinks the branching factor from 18 to 15.
* **Interview Talking Point:** "To solve the massive memory footprint of BFS, I shifted the engine to IDDFS. To mitigate the runtime cost of DFS, I implemented aggressive pruning. Because my Enums are grouped chronologically by face, a simple integer division determines if the next move affects the same face as the previous move. Skipping these redundant sequences collapses the branching factor from $O(18^D)$ to $O(15^D)$, accelerating the solver by multiple orders of magnitude with a single `if` statement."

## 8. IDA* and Admissible Heuristics

**Decision:** We upgraded our blind IDDFS algorithm into IDA* (Iterative Deepening A*). To do this, we implemented a simple heuristic: `misplaced_stickers / 20`. 
* **Why we chose this:** Standard A* uses a Priority Queue which suffers from the exact same fatal memory crash as BFS. IDA* uses the zero-memory footprint of DFS, but bounds the search using `f = g + h`. 
* **The Admissibility Constraint:** To guarantee the optimal shortest path, `h` MUST be admissible (it must never overestimate). Since a single 90-degree physical twist alters exactly 8 face stickers and 12 side stickers (20 total), it is mathematically impossible to fix 40 misplaced stickers in less than 2 moves. Therefore, `misplaced / 20` is guaranteed to be an underestimate, preserving A*'s optimality.
* **Interview Talking Point:** "When transitioning to IDA*, the most critical factor is ensuring the heuristic is strictly admissible to guarantee optimal paths. I implemented a simple heuristic dividing misplaced stickers by 20. Because one move alters at most 20 stickers, it acts as a perfect mathematical floor. This simple addition provided enough "vision" to act as a compass. For a depth-6 scramble, IDDFS explored 5.4 million nodes. My IDA* solver with this weak heuristic explored only 3.7 million nodes—pruning 1.7 million useless states! However, it proved the absolute necessity of building a stronger heuristic via Pattern Databases to scale to 20-move scrambles."

***

## 9. Pattern Databases (The Ultimate Heuristic)

**Decision:** We implemented a Corner Pattern Database (PDB) to provide a mathematically perfect, admissible heuristic up to depth 6 for our IDA* solver.
* **Why we chose this:** Our initial heuristic (`misplaced / 20`) was mathematically capped at 2, meaning it provided almost no "vision" to prune branches in deeper searches. A Pattern Database solves this by pre-computing the exact minimum moves required to solve a sub-state (e.g., the corners) using a backwards Breadth-First Search from the solved state. 
* **Implementation:** We built a standalone `build_pdb.cpp` script that ran a BFS up to depth 6. It found and compressed 1.4 million unique corner permutations into a binary file `corner_pdb.bin`. When our IDA* solver initializes, it loads this binary file into a `std::unordered_map` in RAM.
* **Results:** The performance gains were astronomical. On a 6-move scramble, the blind IDDFS algorithm blindly searched 570,777 nodes. The IDA* solver, armed with the Corner PDB, explored exactly **41 nodes**. It essentially walked straight to the solution with zero wasted effort.
* **Interview Talking Point:** "To scale the IDA* solver, I built a Pattern Database. Since an 18-branch tree grows too fast for weak heuristics, I wrote a script to run a backwards BFS from the solved state, ignoring the edges, to pre-compute the exact optimal path to solve just the corners. I saved these 1.4 million permutations to a binary file, which my solver loads into a hash map at runtime. Because solving the corners is always an underestimate of solving the whole cube, it is strictly admissible. This gave my solver perfect vision, reducing the nodes visited on a depth-6 scramble from 570,000 down to exactly 41."
