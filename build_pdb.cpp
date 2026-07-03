#include <iostream>
#include <queue>
#include <vector>
#include "Models/RubiksCube1dArray.h"
#include "PatternDatabases/CornerPatternDatabase.h"

int main() {
    RubiksCube1dArray start_cube;
    CornerPatternDatabase cornerDB("corner_pdb.bin");
    
    std::queue<std::pair<RubiksCube1dArray, uint8_t>> q;
    q.push({start_cube, 0});
    
    cornerDB.setNumMoves(start_cube.getCornerIndex(), 0);
    
    std::vector<GenericRubiksCube::MOVE> moves = {
        GenericRubiksCube::MOVE::L, GenericRubiksCube::MOVE::LPRIME, GenericRubiksCube::MOVE::L2,
        GenericRubiksCube::MOVE::R, GenericRubiksCube::MOVE::RPRIME, GenericRubiksCube::MOVE::R2,
        GenericRubiksCube::MOVE::U, GenericRubiksCube::MOVE::UPRIME, GenericRubiksCube::MOVE::U2,
        GenericRubiksCube::MOVE::D, GenericRubiksCube::MOVE::DPRIME, GenericRubiksCube::MOVE::D2,
        GenericRubiksCube::MOVE::F, GenericRubiksCube::MOVE::FPRIME, GenericRubiksCube::MOVE::F2,
        GenericRubiksCube::MOVE::B, GenericRubiksCube::MOVE::BPRIME, GenericRubiksCube::MOVE::B2
    };

    // Generating up to Depth 11.
    // This is the absolute mathematical maximum depth for Rubik's Cube corners!
    // It will explore all 88,179,840 states and take ~30-40 minutes.
    int max_depth = 11; 
    
    std::cout << "Starting PDB Generation up to depth " << max_depth << "...\n";
    std::cout << "This runs a backwards BFS from the solved state.\n\n";
    
    int nodes_explored = 0;
    while (!q.empty()) {
        auto curr = q.front();
        RubiksCube1dArray curr_cube = curr.first;
        uint8_t depth = curr.second;
        q.pop();
        
        nodes_explored++;
        if (nodes_explored % 50000 == 0) {
            std::cout << "Explored " << nodes_explored << " nodes. PDB Size: " << cornerDB.getSize() << " unique corners.\n";
        }

        if (depth >= max_depth) {
            continue;
        }

        for (auto move : moves) {
            curr_cube.move(move);
            uint32_t index = curr_cube.getCornerIndex();
            
            // If we have NEVER seen this corner permutation before, we add it!
            if (!cornerDB.hasState(index)) {
                cornerDB.setNumMoves(index, depth + 1);
                q.push({curr_cube, depth + 1});
            }
            
            curr_cube.invert(move);
        }
    }
    
    std::cout << "\nGeneration Complete!\n";
    std::cout << "Total nodes explored: " << nodes_explored << "\n";
    std::cout << "Total unique corner states found: " << cornerDB.getSize() << "\n";
    
    std::cout << "Saving to corner_pdb.bin...\n";
    if (cornerDB.saveDatabase()) {
        std::cout << "Save successful! Your solver now has Vision!\n";
    } else {
        std::cout << "Failed to save!\n";
    }
    
    return 0;
}
