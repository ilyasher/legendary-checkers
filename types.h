#ifndef TYPES_H
#define TYPES_H

#include "move.h"
#include <cstdint>

/** A type to distinguish the two players. */
enum color_t { white, black };

/**
 * A type that stores 64 bits - one for each square on the board.
 * An on bit represents that the square contains a certain type of piece.
 * In checkers, only 32 squares can have pieces on them, so only a 32-bit
 * int is actually needed. But we have lots of memory and not a lot
 * of patience.
 */
using bit_board_t = uint64_t;

#endif // TYPES_H
