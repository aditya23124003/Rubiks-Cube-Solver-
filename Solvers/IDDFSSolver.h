#ifndef IDDFS_SOLVER_H
#define IDDFS_SOLVER_H

#include <vector>
#include "../Models/RubiksCube.h"

template <typename T, typename H>
class IDDFSSolver {
private:
    std::vector<GenericRubiksCube::MOVE> moves;
    int max_search_depth;
    T rubiksCube;

    bool dfs(T& node, int depth, int max_depth, std::vector<GenericRubiksCube::MOVE>& path, uint64_t& nodes_visited) {
        nodes_visited++;
        if (node.isSolved()) {
            return true;
        }
        if (depth >= max_depth) {
            return false;
        }

        for (int i = 0; i < 18; i++) {
            GenericRubiksCube::MOVE curr_move = moves[i];
            
            // Pruning optimization
            if (!path.empty() && ((int)curr_move / 3 == (int)path.back() / 3)) {
                continue; 
            }

            node.move(curr_move);
            path.push_back(curr_move);

            if (dfs(node, depth + 1, max_depth, path, nodes_visited)) {
                return true;
            }

            path.pop_back();
            node.invert(curr_move);
        }
        return false;
    }

public:
    uint64_t nodes_visited;

    IDDFSSolver(T _rubiksCube, int _max_search_depth = 20) {
        rubiksCube = _rubiksCube;
        max_search_depth = _max_search_depth;
        nodes_visited = 0;
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
        nodes_visited = 0;
        
        for (int i = 1; i <= max_search_depth; i++) {
            T start_node = rubiksCube;
            if (dfs(start_node, 0, i, path, nodes_visited)) {
                return path; 
            }
        }
        return path; 
    }
};

#endif
