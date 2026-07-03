#ifndef RUBIKS_CUBE_1D_ARRAY_H
#define RUBIKS_CUBE_1D_ARRAY_H

#include "RubiksCube.h"

class RubiksCube1dArray : public GenericRubiksCube {
private:
    char cube[54];

    // The 6 basic clockwise 90-degree rotations
    void u();
    void l();
    void f();
    void r();
    void b();
    void d();

    // The counter-clockwise and 180-degree rotations
    void uPrime();
    void u2();
    void lPrime();
    void l2();
    void fPrime();
    void f2();
    void rPrime();
    void r2();
    void bPrime();
    void b2();
    void dPrime();
    void d2();

public:
    RubiksCube1dArray();

    COLOR getColor(FACE face, unsigned row, unsigned col) const override;
    bool isSolved() const override;
    std::string getString() const override;
    void print() const override;
    std::vector<MOVE> randomShuffle(unsigned int times) override;
    GenericRubiksCube& move(MOVE ind) override;
    GenericRubiksCube& invert(MOVE ind) override;

    bool operator==(const RubiksCube1dArray &r1) const;
    RubiksCube1dArray &operator=(const RubiksCube1dArray &r1);

    uint8_t getMisplacedStickersCost() const override;
    std::string getCornerStateString() const override;
    uint32_t getCornerIndex() const override;

    const char* getCube() const { return cube; }
};

struct Hash1d {
    size_t operator()(const RubiksCube1dArray& r1) const {
        size_t hash = 0;//size_t for implict modulo
        const char* cube = r1.getCube();
        for (int i = 0; i < 54; i++) {
            hash = hash * 31 + cube[i];
        }
        return hash;
    }
};

#endif // RUBIKS_CUBE_1D_ARRAY_H
