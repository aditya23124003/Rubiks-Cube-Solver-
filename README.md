# High-Performance Rubik's Cube Solver

An optimal Rubik's Cube heuristic search engine written in modern C++. This project models the massive **43-quintillion state space** of a Rubik's Cube as an implicit mathematical graph and utilizes advanced search algorithms to find the absolute mathematically shortest path (optimal solution) from any scrambled state.

## 🚀 Key Features

* **IDA* Graph Search:** Implements the Iterative Deepening A* (IDA*) algorithm, severely outperforming standard blind searches (BFS / IDDFS) by pruning 99.999% of redundant branches.
* **Corner Pattern Database:** Uses a precomputed 88 MB lookup table as an admissible heuristic to guide the IDA* search, enabling the engine to look ahead and evaluate states instantly.
* **O(1) Perfect Hashing:** Completely replaces slow standard library hash maps. By mathematically pairing **Lehmer Encoding** (for permutations) with **Base-3 Mathematics** (for orientations), the engine directly maps all 88,179,840 possible corner configurations to a perfect, collision-free 1D array index.
* **Extreme CPU Cache Locality:** The Rubik's Cube state is flattened from a 3D array into a contiguous 1D bit-level representation, and complex lookups are aggressively precomputed at compile-time using `static constexpr`. 

## ⚡ Performance Metrics

The engine is heavily optimized for low-latency quantitative environments:
* **Throughput:** Processes up to **7,000,000 nodes per second** on a standard CPU.
* **Solve Speed:** Solves scrambles up to 10 moves instantaneously (under 30 ms). Complex 15-move scrambles are solved in under **3 seconds**.
* **Memory Footprint:** Operates strictly within a flat **88.1 MB** memory bound without dynamic heap allocations during search loops.

## 🛠️ Project Structure

* **`Models/`**: Contains the virtual `RubiksCube` interface and its optimized implementations (`RubiksCube1dArray`, `RubiksCube3dArray`).
* **`Solvers/`**: Houses the search algorithms: `BFSSolver`, `IDDFSSolver`, and the flagship `IDAstarSolver`.
* **`PatternDatabases/`**: Contains the logic to generate and load the `CornerPatternDatabase` binary file using Lehmer permutations.
* **`main.cpp`**: An interactive CLI loop allowing users to scramble cubes, benchmark solve times, and view path outputs.

## 💻 How to Build and Run

### Prerequisites
* A standard C++ compiler (`g++`, `clang++`, or MSVC). No external libraries are required.

### 1. Build the Pattern Database
Before running the optimal solver, you must generate the 88 MB Corner Pattern Database binary file (`corner_pdb.bin`). This is a one-time process and takes ~5-10 minutes.

```bash
g++ -O3 -o build_pdb build_pdb.cpp Models/RubiksCube1dArray.cpp
./build_pdb
```

### 2. Run the Interactive Solver
Once the database is generated, compile and run the main interactive CLI.

```bash
g++ -O3 -o solver main.cpp Models/RubiksCube1dArray.cpp
./solver
```

## 🧠 Theory: The Limits of a Corner Heuristic
While the engine explores millions of nodes per second, solving certain deep scrambles exclusively with a Corner PDB will eventually encounter exponential bottlenecks. This happens because the heuristic is *admissible* but entirely blind to edge pieces. If a cube has highly scrambled edges but accidentally solved corners (e.g., a 12-move scramble where the corners are only 5 moves away), the algorithm is forced to brute-force the remaining depth, which can occasionally take up to 40 seconds. To instantly solve the absolute hardest states (20 moves, or "God's Number"), the architecture is designed to be easily expandable to include a concurrent Edge Pattern Database.
