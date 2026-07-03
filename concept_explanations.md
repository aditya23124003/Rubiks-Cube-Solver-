# Rubik's Cube & C++ Concept Explanations

*This file serves as a vault for all the conceptual math, graph theory, and C++ explanations discussed during the project development.*

## 1. The Scale of the Problem & Parity
- **Total States:** 8 Corners can be arranged in 8! ways, with 3^7 orientations. 12 Edges can be arranged in 12! ways, with 2^11 orientations. 
- **The Parity Constraint (Division by 2):** A single 90-degree turn is a 4-cycle of corners AND a 4-cycle of edges. In permutation math, a 4-cycle is an "odd" permutation. Because they happen simultaneously, the parity of corners and edges must always match. You can never have "even" corners and "odd" edges. This divides the reachable states by exactly 2 (resulting in ~4.3 x 10^19 states).

## 2. Move Notation (Singmaster)
- **Clockwise Rule:** Every single-letter move (`U`, `D`, `F`, `B`, `R`, `L`) represents a 90-degree clockwise rotation **as if you are looking directly at that face**.
- Prime moves (`U'`) are counter-clockwise. Double moves (`U2`) are 180 degrees.

## 3. C++ Architecture & OOP
- **Encapsulation:** The initial cube state `T rubiksCube;` inside our solvers is `private`. This ensures that external code cannot accidentally mutate the solver's internal starting state mid-search, which would corrupt the algorithm.
- **Templates:** We used `template <typename T, typename H>` so our solvers are agnostic to the cube's data structure. We can plug in a `RubiksCube1dArray` or a future `RubiksCubeBitboard` without rewriting any BFS/DFS logic.

## 4. State Representation
- **1D Array (`char cube[54]`):** Extremely cache-friendly. The entire 54-byte state fits in a single modern 64-byte L1 CPU cache line, resulting in zero cache misses during execution.
- **Bitboards (`uint64_t`):** The absolute fastest representation used in HFT and professional engines. It uses bitwise shifts to update states in a single CPU cycle, but is mathematically agonizing to write compared to 1D arrays.

## 5. Hashing & Memory Allocation (HFT Concepts)
- **Custom Hashing:** C++ `std::unordered_map` requires a custom hash function for user-defined types. We built a polynomial rolling hash that iterates over the 54-char array.
- **Why no `std::string`?** Calling `cube.getString()` allocates heap memory dynamically. In BFS, where millions of nodes are explored per second, allocating and freeing heap memory destroys performance and causes memory fragmentation. Our manual hash is allocation-free (O(1) memory overhead).
- **Collision Handling:** If two different cubes hash to the same bucket, `std::unordered_map` uses "Separate Chaining" (a linked list) and our `operator==` to walk the list and find the exact cube.

## 6. Graph Search Algorithms (BFS vs DFS)
- **Breadth-First Search (BFS):** Explores level-by-level. Guaranteed to find the optimal/shortest path. However, because the branching factor is 18, it suffers from severe memory explosion (e.g., millions of states in the queue at depth 6).
- **Path Reconstruction Optimization:** To save memory in BFS, we do not store `std::vector<MOVE>` in the queue. Instead, we map `visited[new_state] = move`. When we find the solution, we trace backwards by inverting the moves, reducing path payload memory to exactly 1-byte per node.
- **Depth-First Search (DFS):** Highly memory efficient (O(Depth) memory). However, it is "blind" and will likely find sub-optimal paths (e.g., a 6-move solution when a 5-move exists).
- **DFS Pruning:** A critical optimization. Instead of blindly trying all 18 moves, DFS can look at the previous move. If the previous move was `U`, we immediately skip `U`, `U'`, and `U2` for the next move, cutting the branching factor from 18 to 15.

## 7. Time & Space Complexity Analysis
For graph traversal, we define two variables:
* **b (Branching Factor)**: The number of choices at each node. For a Rubik's cube without pruning, `b = 18`. With our pruning optimization, `b = 15`.
* **d (Depth)**: The depth of the optimal/shortest solution.
* **m (Max Depth)**: The maximum depth limit applied to a search.

### 1. Breadth-First Search (BFS)
* **Time Complexity:** O(b<sup>d</sup>). It explores every node at depth 1, then depth 2, down to depth `d`.
* **Space Complexity:** O(b<sup>d</sup>). It must hold the entire frontier of the search in the queue (and all previously visited states in a hash map). This is the fatal flaw of BFS. At depth 7, it stores millions of states in RAM.

### 2. Depth-First Search (DFS - Depth Limited)
* **Time Complexity:** O(b<sup>m</sup>). It searches every single path up to the maximum depth limit `m`. If the target is at depth 3, but the depth limit is 8, it might search all 15<sup>8</sup> paths before finding it, making it extremely inefficient.
* **Space Complexity:** O(m). It only stores the *current* path being explored in memory. A depth limit of 8 means storing exactly 8 moves in memory. This is basically zero memory overhead.

### 3. Iterative Deepening DFS (IDDFS)
* **Time Complexity:** O(b<sup>d</sup>). Wait, doesn't it repeat work by searching depth 1, then searching depth 1 & 2, then searching 1 & 2 & 3? Yes, but because the tree grows exponentially (powers of 15), the vast majority of nodes exist at the very bottom layer. The mathematical sum of all previous layers combined is insignificant compared to the final layer. The total time evaluates to `O(b^d)`, making it just as fast as BFS.
* **Space Complexity:** O(d). Because it runs DFS under the hood, it inherits the memory efficiency of DFS.

**Conclusion:** IDDFS is the holy grail. It perfectly matches the optimal time complexity of BFS while perfectly matching the zero-memory space complexity of DFS.
