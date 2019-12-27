#include "rulebook.h"

std::vector<square_t> get_squares(bit_board_t board) {
    std::vector<square_t> squares;
    for (square_t square = 0; square < NUM_SQUARES; square ++)
        if (board & 1)
            squares.push_back(square);

    return squares;
}
