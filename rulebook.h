#ifndef RULEBOOK_H
#define RULEBOOK_H

#include "types.h"
#include <vector>

/** The number of squares on the board. */
const square_t NUM_SQUARES = 64;

/** The number of index shifts skipping a row is equivalent to. */
const square_t ROW = 8;

/** The number of index shifts skipping a column is equivalent to. */
const square_t COL = 1;

/**
 * The list of moves a piece going down the board can make.
 * This includes both normal moves and captures.
 * Black men, as well as any color kings can make these moves.
 */
const std::vector<Move> BACKWARD_MOVES = {
    Move{false, 0, -ROW - COL, NONE},
    Move{false, 0, -ROW + COL, NONE},
    Move{false, 0, -2 * ROW - 2 * COL, -ROW - COL},
    Move{false, 0, -2 * ROW + 2 * COL, -ROW + COL}
};

/**
 * The list of moves a piece going up the board can make.
 * This includes both normal moves and captures.
 * White men, as well as any color kings can make these moves.
 */
const std::vector<Move> FORWARD_MOVES = {
    Move{false, 0, ROW + COL, NONE},
    Move{false, 0, ROW - COL, NONE},
    Move{false, 0, 2 * ROW + 2 * COL, ROW + COL},
    Move{false, 0, 2 * ROW - 2 * COL, ROW - COL}
};

/**
 * Return a vector containing each square_t whose corresponding bit
 * is on in the given bit_board_t.
 */
std::vector<square_t> get_squares(bit_board_t board);

/** Returns whether a given square on a given bitboard is on. */
inline bool square_is_on(bit_board_t board, square_t square) {
    return board & (1 << square);
}

#endif // RULEBOOK_H
