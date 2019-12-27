#ifndef POSITION_H
#define POSITION_H

#include "rulebook.h"
#include "types.h"
#include <cstdint>
#include <vector>

/**
 * A class that represents a state of the game.
 */
class Position {

    /**
     * This struct stores which squares hold which type of pieces.
     */
    struct BitBoards {
        bit_board_t white_men;
        bit_board_t black_men;
        bit_board_t white_kings;
        bit_board_t black_kings;
    };

    /** The current state of the board. */
    BitBoards bit_boards;

    /** Whose turn it currently is. */
    color_t turn;

    /**
     * The number of moves that have been played up to this point.
     * Unlike chess, each player's individual plays count as moves, so
     * white playing once and black playing once counts as two moves.
     */
    uint16_t n_moves;

    /**
     * The piece currently capturing other pieces.
     * If this is not NONE, another capture may be played by that piece.
     */
    square_t capturer;

public:

    /** Constructor. Sets up the initial position. */
    Position();

    /** Destructor. */
    ~Position();

    /** Returns which squares hold white men. */
    bit_board_t white_men() const;

    /** Returns which squares hold black men. */
    bit_board_t black_men() const;

    /** Returns which squares hold white kings. */
    bit_board_t white_kings() const;

    /** Returns which squares hold black kings. */
    bit_board_t black_kings() const;

    /** Returns which squares hold white pieces of any type (men or kings). */
    bit_board_t white_pieces() const;

    /** Returns which squares hold black pieces of any type (men or kings). */
    bit_board_t black_pieces() const;

    /** Returns which squares have any pieces at all. */
    bit_board_t pieces() const;

    /** Return all the empty squares. */
    bit_board_t empties() const;

    /**
     * Return a vector of all Moves that can be played by whoever's turn it
     * is in this Position.
     */
    std::vector<Move> get_all_legal_moves() const;

    /**
     * Add all the legal moves of a given piece type to the given list
     * of legal moves.
     * @param legal_moves A reference to the vector the new moves should
     *                    be added to.
     * @param piece_board A bit_board_t containing all the pieces of a certain
     *                    type. All moves these pieces can make will be added
     *                    to legal_moves.
     * @param enemy_board A bit_board_t of enemy piece. These are the pieces
     *                    that can be captured by the pieces in piece_board.
     * @param template_piece_moves A vector of template Moves the a single piece
     *                             can hypothetically make.
     * @param must_capture Whether the move must be a capture to be legal. This
     *                     happens in the middle of a capturing sequence.
     */
    void get_piece_moves(
        std::vector<Move> &legal_moves,
        bit_board_t piece_board,
        bit_board_t enemy_board,
        std::vector<Move> template_piece_moves,
        bool must_capture
    ) const;

    /** Play a given move on this position, modifying it. */
    // void play_move(Move move);

};

#endif // POSITION_H
