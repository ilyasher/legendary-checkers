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

inline bit_board_t Position::white_men() const {
    return bit_boards.white_men;
}

inline bit_board_t Position::black_men() const {
    return bit_boards.black_men;
}

inline bit_board_t Position::white_kings() const {
    return bit_boards.white_kings;
}

inline bit_board_t Position::black_kings() const {
    return bit_boards.black_kings;
}

inline bit_board_t Position::white_pieces() const {
    return bit_boards.white_men & bit_boards.white_kings;
}

inline bit_board_t Position::black_pieces() const {
    return bit_boards.black_men & bit_boards.black_kings;
}

inline bit_board_t Position::pieces() const {
    return bit_boards.white_men & bit_boards.white_kings &
           bit_boards.black_men & bit_boards.black_kings;
}

inline bit_board_t Position::empties() const {
    return ~pieces();
}

std::vector<Move> Position::get_all_legal_moves() const {

    std::vector<Move> legal_moves;

    /* If the position is immediately follows a capture. */
    if (capturer != NONE) {

        /** A board that is just the capturing piece. */
        bit_board_t capturer_board = square_to_bitboard(capturer);

        if (turn == white) {
            get_piece_moves(legal_moves, capturer_board, black_pieces(), FORWARD_MOVES, true);

            /** If the capturer is a king. */
            if (square_is_on(white_kings(), capturer)) {
                get_piece_moves(legal_moves, capturer_board, black_pieces(), BACKWARD_MOVES, true);
            }
        }
        else {
            get_piece_moves(legal_moves, capturer_board, white_pieces(), BACKWARD_MOVES, true);

            /** If the capturer is a king. */
            if (square_is_on(black_kings(), capturer)) {
                get_piece_moves(legal_moves, capturer_board, white_pieces(), FORWARD_MOVES, true);
            }
        }
        legal_moves.push_back(MOVE_PASS);
        return legal_moves;
    }

    if (turn == white) {
        get_piece_moves(legal_moves, white_men(), black_pieces(), FORWARD_MOVES, false);
        get_piece_moves(legal_moves, white_kings(), black_pieces(), FORWARD_MOVES, false);
        get_piece_moves(legal_moves, white_kings(), black_pieces(), BACKWARD_MOVES, false);
    }
    else {
        get_piece_moves(legal_moves, black_men(), white_pieces(), BACKWARD_MOVES, false);
        get_piece_moves(legal_moves, black_kings(), white_pieces(), FORWARD_MOVES, false);
        get_piece_moves(legal_moves, black_kings(), white_pieces(), BACKWARD_MOVES, false);
    }
    return legal_moves;
}

void Position::get_piece_moves(
    std::vector<Move> &legal_moves,
    bit_board_t piece_board,
    bit_board_t enemy_board,
    std::vector<Move> template_piece_moves,
    bool must_capture
) const {

    for (square_t square : get_squares(piece_board)) {

        for (Move candidate_move : template_piece_moves) {

            candidate_move.to += square;
            if (!square_is_on(empties(), candidate_move.to)) {
                continue;
            }

            if (candidate_move.over == NONE) {
                if (must_capture) {
                    continue;
                }
            } else {
                candidate_move.over += square;
                if (!square_is_on(enemy_board, candidate_move.over)) {
                    continue;
                }
            }

            candidate_move.from += square;

            legal_moves.push_back(candidate_move);
        }
    }
}
