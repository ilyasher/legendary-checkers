#include "rulebook.h"
#include <iostream>

std::vector<square_t> get_squares(bit_board_t board) {
    std::vector<square_t> squares;
    for (square_t square = 0; square < NUM_SQUARES; square ++) {
        if (square_is_on(board, square)) {
            squares.push_back(square);
        }
    }

    return squares;
}

std::ostream & operator<<(std::ostream &os, square_t square) {
    char letter = square % 8 + 'a';
    int num = square / 8 + 1;
    os << letter << num;
    return os;
}

std::ostream & operator<<(std::ostream &os, const Move &move) {

    if (move.is_pass) {
        os << "Pass";
        return os;
    }
    if (move.is_capture()) {
        os << "Capture from ";
    }
    else {
        os << "Move from ";
    }
    os << move.from << " to " << move.to << ".";
    return os;
}
