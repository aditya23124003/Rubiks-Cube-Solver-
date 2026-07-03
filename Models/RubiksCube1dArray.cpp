#include "RubiksCube1dArray.h"
#include <iostream>

RubiksCube1dArray::RubiksCube1dArray() {
    // Initialize the cube with standard colors
    // U: WHITE, L: GREEN, F: RED, R: BLUE, B: ORANGE, D: YELLOW
    // According to our array mapping: 0-8:U, 9-17:L, 18-26:F, 27-35:R, 36-44:B, 45-53:D
    for (int i = 0; i < 9; i++) cube[i] = 'W';
    for (int i = 9; i < 18; i++) cube[i] = 'G';
    for (int i = 18; i < 27; i++) cube[i] = 'R';
    for (int i = 27; i < 36; i++) cube[i] = 'B';
    for (int i = 36; i < 45; i++) cube[i] = 'O';
    for (int i = 45; i < 54; i++) cube[i] = 'Y';
}

GenericRubiksCube::COLOR RubiksCube1dArray::getColor(FACE face, unsigned row, unsigned col) const {
    int face_idx = (int)face * 9;
    char c = cube[face_idx + row * 3 + col];
    switch (c) {
        case 'B': return COLOR::BLUE;
        case 'G': return COLOR::GREEN;
        case 'R': return COLOR::RED;
        case 'Y': return COLOR::YELLOW;
        case 'W': return COLOR::WHITE;
        case 'O': return COLOR::ORANGE;
    }
    return COLOR::WHITE;
}

bool RubiksCube1dArray::isSolved() const {
    for (int i = 0; i < 9; i++) if (cube[i] != 'W') return false;
    for (int i = 9; i < 18; i++) if (cube[i] != 'G') return false;
    for (int i = 18; i < 27; i++) if (cube[i] != 'R') return false;
    for (int i = 27; i < 36; i++) if (cube[i] != 'B') return false;
    for (int i = 36; i < 45; i++) if (cube[i] != 'O') return false;
    for (int i = 45; i < 54; i++) if (cube[i] != 'Y') return false;
    return true;
}

std::string RubiksCube1dArray::getString() const {
    return std::string(cube, 54);
}

void RubiksCube1dArray::print() const {
    std::cout << "Rubik's Cube State:\n";
    for (int row = 0; row < 3; row++) {
        std::cout << "      ";
        for (int col = 0; col < 3; col++) std::cout << cube[row * 3 + col] << " ";
        std::cout << "\n";
    }
    for (int row = 0; row < 3; row++) {
        for (int col = 0; col < 3; col++) std::cout << cube[9 + row * 3 + col] << " ";
        for (int col = 0; col < 3; col++) std::cout << cube[18 + row * 3 + col] << " ";
        for (int col = 0; col < 3; col++) std::cout << cube[27 + row * 3 + col] << " ";
        for (int col = 0; col < 3; col++) std::cout << cube[36 + row * 3 + col] << " ";
        std::cout << "\n";
    }
    for (int row = 0; row < 3; row++) {
        std::cout << "      ";
        for (int col = 0; col < 3; col++) std::cout << cube[45 + row * 3 + col] << " ";
        std::cout << "\n";
    }
    std::cout << "\n";
}

void RubiksCube1dArray::u() {
    char t0 = cube[0];
    cube[0] = cube[6];
    cube[6] = cube[8];
    cube[8] = cube[2];
    cube[2] = t0;
    char t1 = cube[1];
    cube[1] = cube[3];
    cube[3] = cube[7];
    cube[7] = cube[5];
    cube[5] = t1;
    char t9 = cube[9];
    cube[9] = cube[18];
    cube[18] = cube[27];
    cube[27] = cube[36];
    cube[36] = t9;
    char t10 = cube[10];
    cube[10] = cube[19];
    cube[19] = cube[28];
    cube[28] = cube[37];
    cube[37] = t10;
    char t11 = cube[11];
    cube[11] = cube[20];
    cube[20] = cube[29];
    cube[29] = cube[38];
    cube[38] = t11;
}
void RubiksCube1dArray::l() {
    char t0 = cube[0];
    cube[0] = cube[44];
    cube[44] = cube[45];
    cube[45] = cube[18];
    cube[18] = t0;
    char t3 = cube[3];
    cube[3] = cube[41];
    cube[41] = cube[48];
    cube[48] = cube[21];
    cube[21] = t3;
    char t6 = cube[6];
    cube[6] = cube[38];
    cube[38] = cube[51];
    cube[51] = cube[24];
    cube[24] = t6;
    char t9 = cube[9];
    cube[9] = cube[15];
    cube[15] = cube[17];
    cube[17] = cube[11];
    cube[11] = t9;
    char t10 = cube[10];
    cube[10] = cube[12];
    cube[12] = cube[16];
    cube[16] = cube[14];
    cube[14] = t10;
}
void RubiksCube1dArray::f() {
    char t6 = cube[6];
    cube[6] = cube[17];
    cube[17] = cube[47];
    cube[47] = cube[27];
    cube[27] = t6;
    char t7 = cube[7];
    cube[7] = cube[14];
    cube[14] = cube[46];
    cube[46] = cube[30];
    cube[30] = t7;
    char t8 = cube[8];
    cube[8] = cube[11];
    cube[11] = cube[45];
    cube[45] = cube[33];
    cube[33] = t8;
    char t18 = cube[18];
    cube[18] = cube[24];
    cube[24] = cube[26];
    cube[26] = cube[20];
    cube[20] = t18;
    char t19 = cube[19];
    cube[19] = cube[21];
    cube[21] = cube[25];
    cube[25] = cube[23];
    cube[23] = t19;
}
void RubiksCube1dArray::r() {
    char t2 = cube[2];
    cube[2] = cube[20];
    cube[20] = cube[47];
    cube[47] = cube[42];
    cube[42] = t2;
    char t5 = cube[5];
    cube[5] = cube[23];
    cube[23] = cube[50];
    cube[50] = cube[39];
    cube[39] = t5;
    char t8 = cube[8];
    cube[8] = cube[26];
    cube[26] = cube[53];
    cube[53] = cube[36];
    cube[36] = t8;
    char t27 = cube[27];
    cube[27] = cube[33];
    cube[33] = cube[35];
    cube[35] = cube[29];
    cube[29] = t27;
    char t28 = cube[28];
    cube[28] = cube[30];
    cube[30] = cube[34];
    cube[34] = cube[32];
    cube[32] = t28;
}
void RubiksCube1dArray::b() {
    char t0 = cube[0];
    cube[0] = cube[29];
    cube[29] = cube[53];
    cube[53] = cube[15];
    cube[15] = t0;
    char t1 = cube[1];
    cube[1] = cube[32];
    cube[32] = cube[52];
    cube[52] = cube[12];
    cube[12] = t1;
    char t2 = cube[2];
    cube[2] = cube[35];
    cube[35] = cube[51];
    cube[51] = cube[9];
    cube[9] = t2;
    char t36 = cube[36];
    cube[36] = cube[42];
    cube[42] = cube[44];
    cube[44] = cube[38];
    cube[38] = t36;
    char t37 = cube[37];
    cube[37] = cube[39];
    cube[39] = cube[43];
    cube[43] = cube[41];
    cube[41] = t37;
}
void RubiksCube1dArray::d() {
    char t15 = cube[15];
    cube[15] = cube[42];
    cube[42] = cube[33];
    cube[33] = cube[24];
    cube[24] = t15;
    char t16 = cube[16];
    cube[16] = cube[43];
    cube[43] = cube[34];
    cube[34] = cube[25];
    cube[25] = t16;
    char t17 = cube[17];
    cube[17] = cube[44];
    cube[44] = cube[35];
    cube[35] = cube[26];
    cube[26] = t17;
    char t45 = cube[45];
    cube[45] = cube[51];
    cube[51] = cube[53];
    cube[53] = cube[47];
    cube[47] = t45;
    char t46 = cube[46];
    cube[46] = cube[48];
    cube[48] = cube[52];
    cube[52] = cube[50];
    cube[50] = t46;
}

void RubiksCube1dArray::uPrime() { u(); u(); u(); }
void RubiksCube1dArray::u2() { u(); u(); }
void RubiksCube1dArray::lPrime() { l(); l(); l(); }
void RubiksCube1dArray::l2() { l(); l(); }
void RubiksCube1dArray::fPrime() { f(); f(); f(); }
void RubiksCube1dArray::f2() { f(); f(); }
void RubiksCube1dArray::rPrime() { r(); r(); r(); }
void RubiksCube1dArray::r2() { r(); r(); }
void RubiksCube1dArray::bPrime() { b(); b(); b(); }
void RubiksCube1dArray::b2() { b(); b(); }
void RubiksCube1dArray::dPrime() { d(); d(); d(); }
void RubiksCube1dArray::d2() { d(); d(); }

GenericRubiksCube& RubiksCube1dArray::move(MOVE ind) {
    switch (ind) {
        case MOVE::U: u(); break;
        case MOVE::UPRIME: uPrime(); break;
        case MOVE::U2: u2(); break;
        case MOVE::L: l(); break;
        case MOVE::LPRIME: lPrime(); break;
        case MOVE::L2: l2(); break;
        case MOVE::F: f(); break;
        case MOVE::FPRIME: fPrime(); break;
        case MOVE::F2: f2(); break;
        case MOVE::R: r(); break;
        case MOVE::RPRIME: rPrime(); break;
        case MOVE::R2: r2(); break;
        case MOVE::B: b(); break;
        case MOVE::BPRIME: bPrime(); break;
        case MOVE::B2: b2(); break;
        case MOVE::D: d(); break;
        case MOVE::DPRIME: dPrime(); break;
        case MOVE::D2: d2(); break;
    }
    return *this;
}

GenericRubiksCube& RubiksCube1dArray::invert(MOVE ind) {
    switch (ind) {
        case MOVE::U: uPrime(); break;
        case MOVE::UPRIME: u(); break;
        case MOVE::U2: u2(); break;
        case MOVE::L: lPrime(); break;
        case MOVE::LPRIME: l(); break;
        case MOVE::L2: l2(); break;
        case MOVE::F: fPrime(); break;
        case MOVE::FPRIME: f(); break;
        case MOVE::F2: f2(); break;
        case MOVE::R: rPrime(); break;
        case MOVE::RPRIME: r(); break;
        case MOVE::R2: r2(); break;
        case MOVE::B: bPrime(); break;
        case MOVE::BPRIME: b(); break;
        case MOVE::B2: b2(); break;
        case MOVE::D: dPrime(); break;
        case MOVE::DPRIME: d(); break;
        case MOVE::D2: d2(); break;
    }
    return *this;
}

std::vector<GenericRubiksCube::MOVE> RubiksCube1dArray::randomShuffle(unsigned int times) {
    std::vector<MOVE> moves_performed;
    srand(time(0));
    for (unsigned int i = 0; i < times; i++) {
        unsigned int selectMove = rand() % 18;
        moves_performed.push_back(static_cast<MOVE>(selectMove));
        this->move(static_cast<MOVE>(selectMove));
    }
    return moves_performed;
}

bool RubiksCube1dArray::operator==(const RubiksCube1dArray &r1) const {
    for (int i = 0; i < 54; i++) {
        if (cube[i] != r1.cube[i]) return false;
    }
    return true;
}

RubiksCube1dArray& RubiksCube1dArray::operator=(const RubiksCube1dArray &r1) {
    for (int i = 0; i < 54; i++) {
        cube[i] = r1.cube[i];
    }
    return *this;
}

uint8_t RubiksCube1dArray::getMisplacedStickersCost() const {
    int misplaced = 0;
    
    // U face (Indices 0-8, Center 4)
    for(int i = 0; i < 9; i++) if (cube[i] != cube[4]) misplaced++;
    
    // L face (Indices 9-17, Center 13)
    for(int i = 9; i < 18; i++) if (cube[i] != cube[13]) misplaced++;

    // F face (Indices 18-26, Center 22)
    for(int i = 18; i < 27; i++) if (cube[i] != cube[22]) misplaced++;

    // R face (Indices 27-35, Center 31)
    for(int i = 27; i < 36; i++) if (cube[i] != cube[31]) misplaced++;

    // B face (Indices 36-44, Center 40)
    for(int i = 36; i < 45; i++) if (cube[i] != cube[40]) misplaced++;

    // D face (Indices 45-53, Center 49)
    for(int i = 45; i < 54; i++) if (cube[i] != cube[49]) misplaced++;

    // A single twist moves exactly 20 stickers (8 face + 12 side).
    // So if 40 stickers are wrong, it must take at LEAST 2 moves to fix.
    // Dividing by 20 perfectly guarantees Admissibility!
    return misplaced / 20;
}

std::string RubiksCube1dArray::getCornerStateString() const {
    std::string s = "";
    int corner_idx[] = {
        0,2,6,8,          // U
        9,11,15,17,       // L
        18,20,24,26,      // F
        27,29,33,35,      // R
        36,38,42,44,      // B
        45,47,51,53       // D
    };
    for (int i : corner_idx) {
        s += cube[i];
    }
    return s;
}

uint32_t RubiksCube1dArray::getCornerIndex() const {
    static constexpr uint8_t corner_slots[8][3] = {
        {6, 18, 11}, // UFL: U, F(Clockwise), L(Anti)
        {8, 27, 20}, // UFR: U, R(Clockwise), F(Anti)
        {2, 36, 29}, // UBR: U, B(Clockwise), R(Anti)
        {0, 9, 38},  // UBL: U, L(Clockwise), B(Anti)
        {45, 17, 24},// DFL: D, L(Clockwise), F(Anti)
        {47, 26, 33},// DFR: D, F(Clockwise), R(Anti)
        {53, 35, 42},// DBR: D, R(Clockwise), B(Anti)
        {51, 44, 15} // DBL: D, B(Clockwise), L(Anti)
    };
    
    uint8_t perm[8];
    uint8_t ori[8];
    
    for (int i = 0; i < 8; i++) {
        char c0 = cube[corner_slots[i][0]];
        char c1 = cube[corner_slots[i][1]];
        char c2 = cube[corner_slots[i][2]];
        
        uint8_t o = 0;
        if (c1 == 'W' || c1 == 'Y') o = 1;
        else if (c2 == 'W' || c2 == 'Y') o = 2;
        ori[i] = o;
        
        bool hasW = (c0=='W' || c1=='W' || c2=='W');
        bool hasR = (c0=='R' || c1=='R' || c2=='R');
        bool hasG = (c0=='G' || c1=='G' || c2=='G');
        bool hasB = (c0=='B' || c1=='B' || c2=='B');
        bool hasO = (c0=='O' || c1=='O' || c2=='O');
        
        uint8_t id;
        if (hasW && hasG && hasR) id = 0;
        else if (hasW && hasR && hasB) id = 1;
        else if (hasW && hasB && hasO) id = 2;
        else if (hasW && hasO && hasG) id = 3;
        else if (!hasW && hasG && hasR) id = 4;
        else if (!hasW && hasR && hasB) id = 5;
        else if (!hasW && hasB && hasO) id = 6;
        else id = 7;
        
        perm[i] = id;
    }
    
    uint32_t orientation_idx = 0;
    uint32_t base = 1;
    for (int i = 6; i >= 0; i--) {
        orientation_idx += ori[i] * base;
        base *= 3;
    }
    
    static constexpr uint32_t fact[] = {1, 1, 2, 6, 24, 120, 720, 5040};
    uint32_t lehmer_idx = 0;
    for (int i = 0; i < 8; i++) {
        uint32_t smaller_count = 0;
        for (int j = i + 1; j < 8; j++) {
            if (perm[j] < perm[i]) {
                smaller_count++;
            }
        }
        lehmer_idx += smaller_count * fact[7 - i];
    }
    
    return (lehmer_idx * 2187) + orientation_idx;
}
