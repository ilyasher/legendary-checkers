#include "position.h"
#include <vector>

using square_t = int8_t;

const int8_t NONE = -1;
const int8_t NUM_SQUARES = 64;
const int8_t ROW = 8;
const int8_t COL = 1;

struct Move {
    bool is_pass;  /**< Whether this move is a pass. */
    square_t from; /**< The square the piece is moving from. */
    square_t to;   /**< The square the piece is moving to. */
    square_t over; /**< If Move is a capture, the square it jumps. Else, NONE. */
};

const std::vector<Move> BLACK_MEN_MOVES = {
    Move{false, 0, -ROW - COL, NONE},
    Move{false, 0, -ROW + COL, NONE},
    Move{false, 0, -2 * ROW - 2 * COL, -ROW - COL},
    Move{false, 0, -2 * ROW + 2 * COL, -ROW + COL}
};

const std::vector<Move> WHITE_MEN_MOVES = {
    Move{false, 0, ROW + COL, NONE},
    Move{false, 0, ROW - COL, NONE},
    Move{false, 0, 2 * ROW + 2 * COL, ROW + COL},
    Move{false, 0, 2 * ROW - 2 * COL, ROW - COL}
};

// TODO: This is really ugly. Fix
const std::vector<Move> KING_MOVES = {
    Move{false, 0, -ROW - COL, NONE},
    Move{false, 0, -ROW + COL, NONE},
    Move{false, 0, -2 * ROW - 2 * COL, -ROW - COL},
    Move{false, 0, -2 * ROW + 2 * COL, -ROW + COL},
    Move{false, 0, ROW + COL, NONE},
    Move{false, 0, ROW - COL, NONE},
    Move{false, 0, 2 * ROW + 2 * COL, ROW + COL},
    Move{false, 0, 2 * ROW - 2 * COL, ROW - COL}
};

void Position::get_piece_moves(
    std::vector<Move> &legal_moves,
    bit_board_t piece_board,
    bit_board_t enemy_board,
    std::vector<Move> template_piece_moves);

std::vector<square_t> get_squares(bit_board_t board);

inline bool get_square(bit_board_t board, square_t square);

std::vector<Move> Position::get_all_legal_moves() {

    std::vector<Move> legal_moves;

    // TODO: add being able to pass if you jumped

    get_piece_moves(legal_moves, white_men(), black_pieces(), WHITE_MEN_MOVES);
    get_piece_moves(legal_moves, black_men(), white_pieces(), BLACK_MEN_MOVES);
    get_piece_moves(legal_moves, white_kings(), black_pieces(), KING_MOVES);
    get_piece_moves(legal_moves, black_kings(), white_pieces(), KING_MOVES);

    return legal_moves;
}

void Position::get_piece_moves(
    std::vector<Move> &legal_moves,
    bit_board_t piece_board,
    bit_board_t enemy_board,
    std::vector<Move> template_piece_moves) {

    for (square_t square : get_squares(piece_board)) {

        for (Move candidate_move : template_piece_moves) {

            candidate_move.to += square;
            if (!get_square(empties(), candidate_move.to)) {
                continue;
            }

            if (candidate_move.over != NONE) {
                candidate_move.over += square;
                if (!get_square(enemy_board, candidate_move.over)) {
                    continue;
                }
            }
            candidate_move.from += square;

            legal_moves.push_back(candidate_move);
        }
    }
}

std::vector<square_t> get_squares(bit_board_t board) {
    std::vector<square_t> squares;
    for (square_t square = 0; square < NUM_SQUARES; square ++)
        if (board & 1)
            squares.push_back(square);

    return squares;
}

// TODO: rename this function
inline bool get_square(bit_board_t board, square_t square) {
    return board & (1 << square);
}
