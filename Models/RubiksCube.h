#ifndef RUBIKS_CUBE_H
#define RUBIKS_CUBE_H

#include <vector>
#include <string>
#include <iostream>
#include <cstdint>

/**
 * A generic abstract base class for all Rubik's Cube models.
 * This guarantees that every model implements the same core operations.
 */
class GenericRubiksCube {
public:
    enum class FACE {
        UP,
        LEFT,
        FRONT,
        RIGHT,
        BACK,
        DOWN
    };

    enum class COLOR {
        WHITE,
        GREEN,
        RED,
        BLUE,
        ORANGE,
        YELLOW
    };

    enum class MOVE {
        L, LPRIME, L2,
        R, RPRIME, R2,
        U, UPRIME, U2,
        D, DPRIME, D2,
        F, FPRIME, F2,
        B, BPRIME, B2
    };

    /*
     * Returns the color of the cell at (row, col) in face.
     * If Rubik's Cube is unfolded, it will look like:
     *             U U U
     *             U U U
     *             U U U
     * L L L       F F F       R R R       B B B
     * L L L       F F F       R R R       B B B
     * L L L       F F F       R R R       B B B
     *             D D D
     *             D D D
     *             D D D
     *
     * Row Number: 0 to 2
     * Col Number: 0 to 2
     */
    virtual COLOR getColor(FACE face, unsigned row, unsigned col) const = 0;

    /*
     * Returns the first letter of the given COLOR as a char.
     */
    static char getColorLetter(COLOR color) {
        switch (color) {
            case COLOR::BLUE:   return 'B';
            case COLOR::GREEN:  return 'G';
            case COLOR::RED:    return 'R';
            case COLOR::YELLOW: return 'Y';
            case COLOR::WHITE:  return 'W';
            case COLOR::ORANGE: return 'O';
        }
        return ' ';
    }

    /*
     * Returns true if the Rubik's Cube is completely solved.
     */
    virtual bool isSolved() const = 0;

    /*
     * Returns the state of the Rubik's Cube in a string format.
     */
    virtual std::string getString() const = 0;

    /*
     * Returns the string representation of a MOVE.
     */
    static std::string getMoveString(MOVE ind) {
        switch (ind) {
            case MOVE::L: return "L";
            case MOVE::LPRIME: return "L'";
            case MOVE::L2: return "L2";
            case MOVE::R: return "R";
            case MOVE::RPRIME: return "R'";
            case MOVE::R2: return "R2";
            case MOVE::U: return "U";
            case MOVE::UPRIME: return "U'";
            case MOVE::U2: return "U2";
            case MOVE::D: return "D";
            case MOVE::DPRIME: return "D'";
            case MOVE::D2: return "D2";
            case MOVE::F: return "F";
            case MOVE::FPRIME: return "F'";
            case MOVE::F2: return "F2";
            case MOVE::B: return "B";
            case MOVE::BPRIME: return "B'";
            case MOVE::B2: return "B2";
        }
        return "";
    }

    /*
     * Print the Rubik's Cube in a planar format.
     */
    virtual void print() const = 0;

    /*
     * Randomly shuffle the cube with 'times' moves and returns the moves applied.
     */
    virtual std::vector<MOVE> randomShuffle(unsigned int times) = 0;

    /*
     * Apply a given move on the cube.
     */
    virtual GenericRubiksCube& move(MOVE ind) = 0;

    /*
     * Invert a move. 
     */
    virtual GenericRubiksCube& invert(MOVE ind) = 0;

    /*
     * Returns a weak admissible heuristic.
     * Counts misplaced stickers and divides by 20 (since a single move affects at most 20 stickers).
     */
    virtual uint8_t getMisplacedStickersCost() const = 0;

    /*
     * Returns a string representing just the 8 corners for Pattern Database indexing.
     */
    virtual std::string getCornerStateString() const = 0;

    /*
     * Returns the perfect hash index of the 8 corners (0 to 88,179,839)
     */
    virtual uint32_t getCornerIndex() const = 0;

    virtual ~GenericRubiksCube() = default;
};

#endif // RUBIKS_CUBE_H
