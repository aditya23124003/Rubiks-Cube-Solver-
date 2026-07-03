#ifndef IDASTAR_SOLVER_H
#define IDASTAR_SOLVER_H

#include "../Models/RubiksCube.h"
#include "../PatternDatabases/CornerPatternDatabase.h"
#include <iostream>
#include <vector>

template <typename T, typename H> class IDAstarSolver {
private:
  std::vector<GenericRubiksCube::MOVE> moves;
  T rubiksCube;
  CornerPatternDatabase cornerDB;

  int ida_star(T &node, int g, int bound,
               std::vector<GenericRubiksCube::MOVE> &path,
               uint64_t &nodes_visited) {
    nodes_visited++;

    // PDB Heuristic Lookup!
    int h = cornerDB.getNumMoves(node.getCornerIndex());
    int f = g + h;

    if (f > bound) {
      return f; // Prune branch! Return the new minimum bound.
    }
    if (node.isSolved()) {
      return -1; // Magic number for solved!
    }

    int min_overshoot = 999999;

    for (int i = 0; i < 18; i++) {
      GenericRubiksCube::MOVE curr_move = moves[i];

      // Branch Pruning (same as IDDFS)
      if (!path.empty() && ((int)curr_move / 3 == (int)path.back() / 3)) {
        continue;
      }

      node.move(curr_move);
      path.push_back(curr_move);

      int t = ida_star(node, g + 1, bound, path, nodes_visited);

      if (t == -1) {
        return -1; // Propagate success
      }
      if (t < min_overshoot) {
        min_overshoot = t;
      }

      // Backtrack
      path.pop_back();
      node.invert(curr_move);
    }
    return min_overshoot;
  }

public:
  uint64_t nodes_visited;

  IDAstarSolver(T _rubiksCube, std::string pdb_filename = "corner_pdb.bin") {
    rubiksCube = _rubiksCube;
    nodes_visited = 0;

    cornerDB = CornerPatternDatabase(pdb_filename);
    cornerDB.loadDatabase();

    moves = {GenericRubiksCube::MOVE::L,      GenericRubiksCube::MOVE::LPRIME,
             GenericRubiksCube::MOVE::L2,     GenericRubiksCube::MOVE::R,
             GenericRubiksCube::MOVE::RPRIME, GenericRubiksCube::MOVE::R2,
             GenericRubiksCube::MOVE::U,      GenericRubiksCube::MOVE::UPRIME,
             GenericRubiksCube::MOVE::U2,     GenericRubiksCube::MOVE::D,
             GenericRubiksCube::MOVE::DPRIME, GenericRubiksCube::MOVE::D2,
             GenericRubiksCube::MOVE::F,      GenericRubiksCube::MOVE::FPRIME,
             GenericRubiksCube::MOVE::F2,     GenericRubiksCube::MOVE::B,
             GenericRubiksCube::MOVE::BPRIME, GenericRubiksCube::MOVE::B2};
  }

  std::vector<GenericRubiksCube::MOVE> solve() {
    std::vector<GenericRubiksCube::MOVE> path;
    nodes_visited = 0;

    // Start the bound at the initial heuristic estimate!
    int bound = cornerDB.getNumMoves(rubiksCube.getCornerIndex());

    while (true) {
      T start_node = rubiksCube;
      int t = ida_star(start_node, 0, bound, path, nodes_visited);

      if (t == -1) {
        return path; // Found the solution!
      }
      if (t == 999999) {
        return path; // Impossible
      }
      // If not found, increase the bound to the lowest overshoot encountered
      bound = t;
    }
  }
};

#endif
