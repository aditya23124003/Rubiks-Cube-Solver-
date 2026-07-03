#ifndef CORNER_PATTERN_DATABASE_H
#define CORNER_PATTERN_DATABASE_H

#include <vector>
#include <fstream>
#include <iostream>
#include <cstdint>

class CornerPatternDatabase {
private:
    std::vector<uint8_t> database;
    std::string filename;
    static constexpr uint32_t DB_SIZE = 88179840;

public:
    CornerPatternDatabase(std::string _filename = "corner_pdb.bin") : filename(_filename) {
        // Initialize the massive array with 0xFF (255) to represent "unexplored states"
        database.assign(DB_SIZE, 0xFF);
    }

    void setNumMoves(uint32_t index, uint8_t numMoves) {
        database[index] = numMoves;
    }

    bool hasState(uint32_t index) const {
        return database[index] != 0xFF;
    }

    uint8_t getNumMoves(uint32_t index) const {
        if (database[index] != 0xFF) {
            return database[index];
        }
        // If not found, it means the state is deeper than our PDB's max depth.
        // We will build this up to depth 11 (max possible), but safely return 12 if somehow missed.
        return 12; 
    }

    size_t getSize() const {
        size_t count = 0;
        for (uint8_t val : database) {
            if (val != 0xFF) count++;
        }
        return count;
    }

    bool loadDatabase() {
        std::ifstream infile(filename, std::ios::in | std::ios::binary);
        if (!infile.is_open()) return false;

        // Load the entire 88MB array straight into memory in one fell swoop!
        infile.read(reinterpret_cast<char*>(database.data()), DB_SIZE);
        infile.close();
        return true;
    }

    bool saveDatabase() const {
        std::ofstream outfile(filename, std::ios::out | std::ios::binary);
        if (!outfile.is_open()) return false;

        // Save the entire 88MB array straight to disk
        outfile.write(reinterpret_cast<const char*>(database.data()), DB_SIZE);
        outfile.close();
        return true;
    }
};

#endif
