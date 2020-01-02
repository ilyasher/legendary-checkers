#include "rulebook.h"
#include <iostream>

uint8_t square_to_index(square_t square) {
    for (uint8_t i = 0; i < NUM_SQUARES; i ++) {
        if (square == 1) {
            return i;
        }
        square >>= 1;
    }
}

std::ostream & print_square(std::ostream &os, square_t square) {
    auto index = square_to_index(square);
    char letter = index % 8 + 'a';
    int num = index / 8 + 1;
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
    print_square(os, move.from);
    os << " to ";
    print_square(os, move.to);
    os << ".";
    return os;
}
