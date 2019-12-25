#include "position.h"

using square = int8_t;

const int8_t NONE = -1;

struct Move {
    bool is_pass; /**< Whether this move is a pass. */
    square from;  /**< The square the piece is moving from. */
    square to;    /**< The square the piece is moving to. */
    square over;  /**< If Move is a capture, the square it jumps. Else, NONE. */
};

// set<Move> Position::get_all_legal_moves();
