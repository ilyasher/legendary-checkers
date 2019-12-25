#include "position.h"

int main() {
    return 0;
}

Position::Position() {
    bit_boards = BitBoards{0, 0, 0, 0};

    /* Set up the initial pieces in their initial positions. */
    bit_boards.black_men = 0xaa55aaL << (5 * 8); /* L specifies 64-bit int.  */
    bit_boards.white_men = 0x55aa55; /* 0xaa = 0b10101010, 0x55 = 0b01010101 */

    turn = black;
    n_moves = 0;
}

Position::~Position() {}

inline bit_board Position::white_men() const {
    return bit_boards.white_men;
}

inline bit_board Position::black_men() const {
    return bit_boards.black_men;
}

inline bit_board Position::white_kings() const {
    return bit_boards.white_kings;
}

inline bit_board Position::black_kings() const {
    return bit_boards.black_kings;
}

inline bit_board Position::white_pieces() const {
    return bit_boards.white_men & bit_boards.white_kings;
}

inline bit_board Position::black_pieces() const {
    return bit_boards.black_men & bit_boards.black_kings;
}

inline bit_board Position::pieces() const {
    return bit_boards.white_men & bit_boards.white_kings &
           bit_boards.black_men & bit_boards.black_kings;
}
