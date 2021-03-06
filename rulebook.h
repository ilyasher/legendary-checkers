#ifndef RULEBOOK_H
#define RULEBOOK_H

#include "types.h"
#include <ostream>
#include <vector>

/** The number of squares on the board. */
const uint8_t NUM_SQUARES = 64;

/** The number of index shifts skipping a row is equivalent to. */
const square_t ROW = 8;

/** The number of index shifts skipping a column is equivalent to. */
const square_t COL = 1;

/**
 * Bitboards representing which squares can and can't hold pieces.
 * In checkers, only half of the squares can hold pieces.
 */
const bit_board_t LEGAL_SQUARES   = 0xaa55aa55aa55aa55;
const bit_board_t ILLEGAL_SQUARES = ~LEGAL_SQUARES;

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

/** Returns a bit_board_t with only one square on. */
inline bit_board_t square_index_to_bitboard(square_t square) {
    return 1L << square;
}

/** Returns the index of the turned on bit. */
uint8_t square_to_index(square_t square);

/**
 * Returns a square_t a given number of indices over
 * from a given square_t.
 */
inline square_t left_shift_square(square_t square, int8_t index) {
    if (index >= 0) {
        return square << index;
    }
    return square >> -index;
}

/** Returns whether a given square on a given bitboard is on. */
inline bool square_is_on(bit_board_t board, square_t square) {
    return board & square;
}

/** Returns the color_t opposite of the given color_t `turn`. */
inline color_t switch_turn(color_t turn) {
    if (turn == WHITE) {
        return BLACK;
    }
    return WHITE;
}

/** Turns on a square on a given bit_board_t, passed in by reference. */
inline void add_square(bit_board_t &board, square_t square) {
    board |= square;
}

/** Turns off a square on a given bit_board_t, passed in by reference. */
inline void remove_square(bit_board_t &board, square_t square) {
    board &= ~square;
}

/**
 * Implements stream-output for the square type.
 * Prints the square in the format XY,
 * where X is a letter A-H representing the row,
 * and Y is a number 1-8 representing the column the square is in.
 * Note: this does not overload the << operator, it is a separate function.
 */
std::ostream & print_square(std::ostream &os, square_t square);

/**
 * Implements stream-output for the Move type.
 * Prints out the squares that the piece moves from and to.
 * Indicates whether it is a normal move, capture, or pass.
 */
std::ostream & operator<<(std::ostream &os, const Move &move);

#endif // RULEBOOK_H
