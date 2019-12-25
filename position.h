#include <cstdint>

/** A type to distinguish the two players. */
enum color { white, black };

/**
 * A type that stores 64 bits - one for each square on the board.
 * An on bit represents that the square contains a certain type of piece.
 * In checkers, only 32 squares can have pieces on them, so only a 32-bit
 * int is actually needed. But we have lots of memory and not a lot
 * of patience.
 */
using bit_board = uint64_t;

/**
 * A class that represents a state of the game.
 */
class Position {

    /**
     * This struct stores which squares hold which type of pieces.
     */
    struct BitBoards {
        bit_board white_men;
        bit_board black_men;
        bit_board white_kings;
        bit_board black_kings;
    };

    /** The current state of the board. */
    BitBoards bit_boards;

    /** Whose turn it currently is. */
    color turn;

    /**
     * The number of moves that have been played up to this point.
     * Unlike chess, each player's individual plays count as moves, so
     * white playing once and black playing once counts as two moves.
     */
    uint16_t n_moves;

public:

    /** Constructor. Sets up the initial position. */
    Position();

    /** Destructor. */
    ~Position();

    /** Returns which squares hold white men. */
    bit_board white_men() const;

    /** Returns which squares hold black men. */
    bit_board black_men() const;

    /** Returns which squares hold white kings. */
    bit_board white_kings() const;

    /** Returns which squares hold black kings. */
    bit_board black_kings() const;

    /** Returns which squares hold white pieces of any type (men or kings). */
    bit_board white_pieces() const;

    /** Returns which squares hold black pieces of any type (men or kings). */
    bit_board black_pieces() const;

    /** Returns which squares have any pieces at all. */
    bit_board pieces() const;

};
