#include "position.h"
#include <iostream>

Position::Position() {
    bit_boards = BitBoards{0, 0, 0, 0};

    /* Set up the initial pieces in their initial positions. */
    bit_boards.black_men = 0xaa55aaL << (5 * 8); /* L specifies 64-bit int.  */
    bit_boards.white_men = 0x55aa55; /* 0xaa = 0b10101010, 0x55 = 0b01010101 */

    turn = black;
    n_moves = 0;
    capturer = NONE;
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
    return bit_boards.white_men | bit_boards.white_kings;
}

inline bit_board_t Position::black_pieces() const {
    return bit_boards.black_men | bit_boards.black_kings;
}

inline bit_board_t Position::pieces() const {
    return bit_boards.white_men | bit_boards.white_kings |
           bit_boards.black_men | bit_boards.black_kings;
}

inline bit_board_t Position::empties() const {
    return ~pieces() & LEGAL_SQUARES;
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

    /* Add moves for every piece. */
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

    /* If you can't play any move, you can pass. */
    if (legal_moves.empty()) {
        legal_moves.push_back(MOVE_PASS);
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

            /* Pieces must move to an empty square. */
            if (!square_is_on(empties(), candidate_move.to)) {
                continue;
            }

            if (candidate_move.over == NONE) {
                /* Capture sequence must only contain captures. */
                if (must_capture) {
                    continue;
                }
            } else {
                candidate_move.over += square;

                /* Check that there is an enemy to capture. */
                if (!square_is_on(enemy_board, candidate_move.over)) {
                    continue;
                }
            }

            candidate_move.from += square;

            legal_moves.push_back(candidate_move);
        }
    }
}

Position Position::play_move(const Move &move) const {
    Position new_position = *this;

    /* Unless the move is part of a capture sequence, switch whose turn it is. */
    if (!move.is_capture()) {
        new_position.turn = switch_turn(turn);
        new_position.n_moves ++;
    }

    if (move.is_pass) {
        new_position.capturer = NONE;
        return new_position;
    }

    // maybe a way to make this better? like with get_all_legal_moves
    if (turn == white) {
        if (square_is_on(bit_boards.white_men, move.from)) {
            add_square(new_position.bit_boards.white_men, move.to);
            remove_square(new_position.bit_boards.white_men, move.from);
        }
        else {
            add_square(new_position.bit_boards.white_kings, move.to);
            remove_square(new_position.bit_boards.white_kings, move.from);
        }
        if (move.is_capture()) {
            remove_square(new_position.bit_boards.black_men, move.over);
            remove_square(new_position.bit_boards.black_kings, move.over);
        }
    }
    else {
        if (square_is_on(bit_boards.black_men, move.from)) {
            add_square(new_position.bit_boards.black_men, move.to);
            remove_square(new_position.bit_boards.black_men, move.from);
        }
        else {
            add_square(new_position.bit_boards.black_kings, move.to);
            remove_square(new_position.bit_boards.black_kings, move.from);
        }
        if (move.is_capture()) {
            remove_square(new_position.bit_boards.white_men, move.over);
            remove_square(new_position.bit_boards.white_kings, move.over);
        }
    }

    if (move.is_capture()) {
        new_position.capturer = move.to;
    }

    /* If on the last row, promote to a king. */
    if (move.to / ROW == 7 && square_is_on(bit_boards.white_men, move.from)) {
        remove_square(new_position.bit_boards.white_men, move.to);
        add_square(new_position.bit_boards.white_kings, move.to);
    }
    else if (move.to / ROW == 0 && square_is_on(bit_boards.black_men, move.from)) {
        remove_square(new_position.bit_boards.black_men, move.to);
        add_square(new_position.bit_boards.black_kings, move.to);
    }

    return new_position;
}

bool Position::operator==(const Position &pos) const {
    bool equal = true;
    equal &= bit_boards.white_men == pos.bit_boards.white_men;
    equal &= bit_boards.white_kings == pos.bit_boards.white_kings;
    equal &= bit_boards.black_men == pos.bit_boards.black_men;
    equal &= bit_boards.black_kings == pos.bit_boards.black_kings;
    equal &= turn == pos.turn;
    equal &= n_moves == pos.n_moves;
    equal &= capturer == pos.capturer;
    return equal;
}

bool Position::operator!=(const Position &pos) const {
    return !(*this == pos);
}


std::ostream & operator<<(std::ostream &os, const Position &pos) {

    square_t square = 0;

    for (int row = 0; row < 8; row ++) {

        if (row == 0)
            os << "   *---*---*---*---*---*---*---*---*\n";
        else
            os << "   *--- --- --- --- --- --- --- ---*\n";

        for (int col = 0; col < 8; col ++) {

            if (col == 0)
                os << " " << row + 1 << " ";

            os << "| ";

            char to_print = ' ';
            if (square_is_on(pos.white_men(), square))
                to_print = 'O';
            if (square_is_on(pos.black_men(), square))
                to_print = 'X';
            if (square_is_on(pos.white_kings(), square))
                to_print = '@';
            if (square_is_on(pos.black_kings(), square))
                to_print = '%';

            os << to_print;
            os << " ";

            square ++;
        }
        os << "|\n";

    }
    os << "   *---*---*---*---*---*---*---*---*\n";
    os << "     A   B   C   D   E   F   G   H  \n";
    return os;
}
