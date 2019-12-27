#ifndef RULEBOOK_H
#define RULEBOOK_H

#include "types.h"
#include <vector>

const int8_t NUM_SQUARES = 64;
const int8_t ROW = 8;
const int8_t COL = 1;

const std::vector<Move> BACKWARD_MOVES = {
    Move{false, 0, -ROW - COL, NONE},
    Move{false, 0, -ROW + COL, NONE},
    Move{false, 0, -2 * ROW - 2 * COL, -ROW - COL},
    Move{false, 0, -2 * ROW + 2 * COL, -ROW + COL}
};

const std::vector<Move> FORWARD_MOVES = {
    Move{false, 0, ROW + COL, NONE},
    Move{false, 0, ROW - COL, NONE},
    Move{false, 0, 2 * ROW + 2 * COL, ROW + COL},
    Move{false, 0, 2 * ROW - 2 * COL, ROW - COL}
};

std::vector<square_t> get_squares(bit_board_t board);

// TODO: rename this function
inline bool get_square(bit_board_t board, square_t square) {
    return board & (1 << square);
}

#endif // RULEBOOK_H
