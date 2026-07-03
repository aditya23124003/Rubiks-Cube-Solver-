#ifndef BFS_SOLVER_H
#define BFS_SOLVER_H

#include <vector>
#include <queue>
#include <unordered_map>
#include <algorithm>
#include "../Models/RubiksCube.h"

/**
 * Breadth-First Search (BFS) Solver
 * Guarantees finding the optimal (shortest) solution.
 * Warning: High memory usage due to branching factor of 18.
 */
template <typename T, typename H>
class BFSSolver {
private:
    std::vector<GenericRubiksCube::MOVE> moves;
    std::unordered_map<T, GenericRubiksCube::MOVE, H> visited;
    T rubiksCube;

public:
    BFSSolver(T _rubiksCube) {
        rubiksCube = _rubiksCube;
        moves = {
            GenericRubiksCube::MOVE::L, GenericRubiksCube::MOVE::LPRIME, GenericRubiksCube::MOVE::L2,
            GenericRubiksCube::MOVE::R, GenericRubiksCube::MOVE::RPRIME, GenericRubiksCube::MOVE::R2,
            GenericRubiksCube::MOVE::U, GenericRubiksCube::MOVE::UPRIME, GenericRubiksCube::MOVE::U2,
            GenericRubiksCube::MOVE::D, GenericRubiksCube::MOVE::DPRIME, GenericRubiksCube::MOVE::D2,
            GenericRubiksCube::MOVE::F, GenericRubiksCube::MOVE::FPRIME, GenericRubiksCube::MOVE::F2,
            GenericRubiksCube::MOVE::B, GenericRubiksCube::MOVE::BPRIME, GenericRubiksCube::MOVE::B2
        };
    }

    std::vector<GenericRubiksCube::MOVE> solve() {
        std::queue<T> q;
        q.push(rubiksCube);
        
        // We use the start node as a special marker. The move stored doesn't matter 
        // because we stop reconstructing when curr == rubiksCube.
        visited[rubiksCube] = GenericRubiksCube::MOVE::L; 

        while (!q.empty()) {
            T node = q.front();
            q.pop();

            if (node.isSolved()) {
                // Reconstruct the path backwards
                T curr = node;
                std::vector<GenericRubiksCube::MOVE> path;
                while (!(curr == rubiksCube)) {
                    GenericRubiksCube::MOVE last_move = visited[curr];
                    path.push_back(last_move);
                    curr.invert(last_move); // Trace back to parent state
                }
                
                // We built it backwards, so reverse it
                std::reverse(path.begin(), path.end());
                return path;
            }

            // Generate all 18 neighbors
            for (int i = 0; i < 18; i++) {
                GenericRubiksCube::MOVE curr_move = moves[i];
                T neighbor = node;
                neighbor.move(curr_move);

                if (visited.count(neighbor) == 0) {
                    visited[neighbor] = curr_move;
                    q.push(neighbor);
                }
            }
        }
        return {}; // No solution found (only happens if impossible or out of memory)
    }
};

#endif
