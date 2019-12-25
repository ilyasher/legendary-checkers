#include "position.h"

int main() {
    return 0;
}

inline bit_board Position::white_men() {
    return bit_boards.white_men;
}

inline bit_board Position::black_men() {
    return bit_boards.black_men;
}

inline bit_board Position::white_kings() {
    return bit_boards.white_kings;
}

inline bit_board Position::black_kings() {
    return bit_boards.black_kings;
}

inline bit_board Position::white_pieces() {
    return bit_boards.white_men & bit_boards.white_kings;
}

inline bit_board Position::black_pieces() {
    return bit_boards.black_men & bit_boards.black_kings;
}

inline bit_board Position::pieces() {
    return bit_boards.white_men & bit_boards.white_kings &
           bit_boards.black_men & bit_boards.black_kings;
}
