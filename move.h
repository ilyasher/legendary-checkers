#ifndef MOVE_H
#define MOVE_H

#include <cstdint>

/**
 * A type that represents a position of a square on a board.
 * The values should be either between 0 and 63, or NONE.
 */
using square_t = int8_t;

/** A NULL value for a square_t, i.e. a square_t that is not on the board */
const square_t NONE = -1;

/** A struct that represents a single move in the game. */
struct Move {
    bool is_pass;  /**< Whether this move is a pass. */
    square_t from; /**< The square the piece is moving from. */
    square_t to;   /**< The square the piece is moving to. */
    square_t over; /**< If Move is a capture, the square it jumps. Else, NONE. */
};

const Move MOVE_PASS = Move{true, NONE, NONE, NONE};

#endif // MOVE_H
