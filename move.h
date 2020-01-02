#ifndef MOVE_H
#define MOVE_H

#include <cstdint>

/**
 * A type that represents a position of a square on a board.
 * The values are 64 bit ints, with the bit at the index of the square
 * turned on and the rest turned off. 
 */
using square_t = uint64_t;

/** A NULL value for a square_t, i.e. a square_t that is not on the board */
const square_t NONE = 0;

/** A struct that represents a single move in the game. */
struct Move {
    bool is_pass;  /**< Whether this move is a pass. */
    square_t from; /**< The square the piece is moving from. */
    square_t to;   /**< The square the piece is moving to. */
    square_t over; /**< If Move is a capture, the square it jumps. Else, NONE. */

    /** Returns whether the move is a capture. */
    bool is_capture() const { return over != NONE; }
};

/**
 * The pass move.
 * Played at the end of a capture sequence, or when there are no legal moves.
 */
const Move MOVE_PASS = Move{true, NONE, NONE, NONE};

#endif // MOVE_H
