#ifndef DFS_SOLVER_H
#define DFS_SOLVER_H

#include <vector>
#include "../Models/RubiksCube.h"

/**
 * Depth-First Search (DFS) Solver
 * Memory efficient, but blind. Finds ANY path up to max_search_depth.
 * It will NOT guarantee the shortest path.
 */
template <typename T, typename H>
class DFSSolver {
private:
    std::vector<GenericRubiksCube::MOVE> moves;
    int max_search_depth;
    T rubiksCube; // The start state

    // We do not strictly need a hash map since depth is bounded,
    // but passing the previous move avoids immediate 180 reverses.
    
    bool dfs(T& node, int depth, std::vector<GenericRubiksCube::MOVE>& path) {
        if (node.isSolved()) {
            return true;
        }
        if (depth >= max_search_depth) {
            return false;
        }

        for (int i = 0; i < 18; i++) {
            GenericRubiksCube::MOVE curr_move = moves[i];
            
            node.move(curr_move);
            path.push_back(curr_move);

            if (dfs(node, depth + 1, path)) {
                return true; // Found a solution!
            }

            // Backtrack
            path.pop_back();
            node.invert(curr_move);
        }
        return false;
    }

public:
    DFSSolver(T _rubiksCube, int _max_search_depth = 7) {
        rubiksCube = _rubiksCube;
        max_search_depth = _max_search_depth;
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
        std::vector<GenericRubiksCube::MOVE> path;
        T start_node = rubiksCube;
        dfs(start_node, 0, path);
        return path;
    }
};

#endif
