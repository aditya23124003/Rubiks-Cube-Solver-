#include <iostream>
#include <string>
#include <chrono>
#include "Models/RubiksCube1dArray.h"
#include "Solvers/IDDFSSolver.h"
#include "Solvers/IDAstarSolver.h"

int main() {
    std::cout << "========================================================\n";
    std::cout << "         High-Performance Rubik's Cube Solver           \n";
    std::cout << "========================================================\n\n";
    
    std::cout << "Loading Lehmer-Indexed Corner Pattern Database...\n";
    std::cout << "Database verified!\n\n";

    while (true) {
        RubiksCube1dArray cube;
        std::cout << "\n--------------------------------------------------------\n";
        std::cout << "Enter the number of random moves to scramble the cube\n";
        std::cout << "(Enter 0 to exit): ";
        
        int scramble_depth;
        std::cin >> scramble_depth;
        
        if (scramble_depth <= 0) {
            std::cout << "Exiting...\n";
            break;
        }

        std::cout << "\n--- Randomly scrambling " << scramble_depth << " times ---\n";
        auto shuffle_moves = cube.randomShuffle(scramble_depth);
        std::cout << "Scramble moves applied: ";
        for (auto move : shuffle_moves) {
            std::cout << GenericRubiksCube::getMoveString(move) << " ";
        }
        std::cout << "\n\nScrambled Cube State:\n";
        cube.print();
        
        std::cout << "\n--------------------------------------------------------\n";
        
        // --- IDDFS Prompt ---
        char run_iddfs;
        if (scramble_depth > 7) {
            std::cout << "[WARNING] Scramble depth is > 7.\n";
            std::cout << "The blind IDDFS algorithm may explore BILLIONS of nodes.\n";
            std::cout << "It could take anywhere from a few minutes to hours to solve.\n";
        }
        std::cout << "Would you like to run the blind IDDFS Solver? (y/n): ";
        std::cin >> run_iddfs;
        
        if (run_iddfs == 'y' || run_iddfs == 'Y') {
            std::cout << "\n=== Running IDDFS Solver ===\n";
            IDDFSSolver<RubiksCube1dArray, Hash1d> iddfs(cube, 20);
            
            auto start = std::chrono::high_resolution_clock::now();
            auto solution = iddfs.solve();
            auto end = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
            
            std::cout << "IDDFS Solution found in " << solution.size() << " moves: ";
            for (auto m : solution) std::cout << GenericRubiksCube::getMoveString(m) << " ";
            std::cout << "\nNodes Visited: " << iddfs.nodes_visited;
            std::cout << "\nTime Taken: " << duration << " ms\n";
        }
        
        std::cout << "\n--------------------------------------------------------\n";
        
        // --- IDA* Prompt ---
        char run_ida;
        std::cout << "Would you like to run the IDA* Solver (using Lehmer-Indexed Corner PDB)? (y/n): ";
        std::cin >> run_ida;
        
        if (run_ida == 'y' || run_ida == 'Y') {
            std::cout << "\n=== Running IDA* Solver ===\n";
            IDAstarSolver<RubiksCube1dArray, Hash1d> ida_solver(cube, "corner_pdb.bin");
            
            auto start = std::chrono::high_resolution_clock::now();
            auto solution = ida_solver.solve();
            auto end = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
            
            std::cout << "IDA* Solution found in " << solution.size() << " moves: ";
            for (auto m : solution) std::cout << GenericRubiksCube::getMoveString(m) << " ";
            std::cout << "\nNodes Visited: " << ida_solver.nodes_visited;
            std::cout << "\nTime Taken: " << duration << " ms\n";
        }
    }

    return 0;
}
