
enum color { white, black };

class Position {

    using bit_board = uint64_t;
    struct BitBoards {
        bit_board white_men;
        bit_board black_men;
        bit_board white_kings;
        bit_board black_kings;
    }

    BitBoards bit_boards;

    color turn;

    uint16_t n_moves;

public:

    bit_board white_men() {
        return bit_boards.white_men;
    }

    bit_board black_men() {
        return bit_boards.black_men;
    }

    bit_board white_kings() {
        return bit_boards.white_kings;
    }

    bit_board black_kings() {
        return bit_boards.black_kings;
    }

    bit_board white_pieces() {
        return bit_boards.white_men & bit_boards.white_kings;
    }

    bit_board black_pieces() {
        return bit_boards.black_men & bit_boards.black_kings;
    }

    bit_board pieces() {
        return bit_boards.white_men & bit_boards.white_kings &
               bit_boards.black_men & bit_boards.black_kings;
    }

}
