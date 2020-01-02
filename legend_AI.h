#ifndef LEGEND_AI_H
#define LEGEND_AI_H

#include <random>
#include <cmath>
#include <iostream>
#include <ctime>
#include <limits>
#include "position.h"

/**
 * A type representing an evaluation of the board.
 * A higher (positive) score means white is better, and
 * a lower (negative) score means black is better.
 */
typedef int64_t score_t;

// TODO: Use INT_MAX and INT_MIN
const score_t WHITE_WIN = 100000;
const score_t BLACK_WIN = -100000;

const double MIN_DOUBLE = std::numeric_limits<double>::lowest();

class LegendAI {

    /** A node in the game tree. */
    struct MCTS_Node {

        /** This node's parent node. Value is nullptr if this is the root. */
        MCTS_Node *parent;

        /** A pointer to a heap-allocated Position that this node represents. */
        Position *position;

        /** The number of moves that can be played in this position. */
        int num_moves;

        /** How many times this subtree has been explored. */
        int num_visits;

        /** The sum of all the evaluations of positions in this subtree. */
        score_t total_score;

        /** A vector of every move that can be played in this position. */
        std::vector<Move> moves;

        /**
         * This node's children.
         * These are the positions that can be immediately reached from playing
         * one move in this position.
         */
        std::vector<MCTS_Node *> children;

        /** Constructor. */
        MCTS_Node(Position *pos, MCTS_Node *parent = nullptr);

        /**
         * Returns which child is the most promising move.
         * @param Cp How much exploration should be considered vs exploitation.
         *           In other words, how open this should be to considering
         *           positions that are not well-explored in favor of positions
         *           that have a high average evaluation score.
         *           This value should roughly be about the evaluation score
         *           that is considered winning strongly.
         */
        MCTS_Node *best_child(double Cp);

        /**
         * Looks for the given position in this tree. Returns that subtree and
         * deletes the rest of this tree.
         */
        MCTS_Node *find_explored_branch(Position *pos);

        /** Returns whether this node has no children (yet). */
        inline bool is_leaf() { return children.empty(); }

        /** Returns whether all moves from this position have been explored. */
        inline bool is_fully_expanded() { return children.size() == num_moves; }
    };

    /**
     * The root of the game tree. The AI tries to find the best move from the
     * position in this node.
     */
    MCTS_Node *game_tree;

    /** Returns which node should be the next one to be further explored. */
    MCTS_Node *select_node(MCTS_Node *node);

    /** Explores one of the unexplored children of a given node. */
    MCTS_Node *expand_node(MCTS_Node *node);

    /** Reports the evaluation of the newly-explored node up the tree. */
    void back_propogate(MCTS_Node *node, score_t score);

public:

    /** Constructor. */
    LegendAI();

    /** Returns a very simple evaluation score of the given position. */
    score_t eval(Position *pos);

    /**
     * Returns the Move that the AI thinks is best in the given position.
     * @param pos The position to analyze.
     * @param time The number of seconds given to make a decision.
     */
    Move best_move(Position &pos, double time = 0.5);

};

/** Returns a random number from 0 to `max`. */
int rand_range(int max);

#endif // LEGEND_AI_H
