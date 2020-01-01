#ifndef LEGEND_AI_H
#define LEGEND_AI_H

#include <random>
#include <cmath>
#include <iostream>
#include <ctime>
#include <limits>
#include "position.h"

using score_t = int32_t;

// TODO: Use INT_MAX and INT_MIN
const score_t WHITE_WIN = 100000;
const score_t BLACK_WIN = -100000;

const double MIN_DOUBLE = std::numeric_limits<double>::lowest();

class LegendAI {

    // ideas: cache game states

    struct MCTS_Node {
        MCTS_Node *parent;
        Position *position;
        int num_moves;

        int num_visits;
        int64_t total_score;

        std::vector<MCTS_Node *> children;

        MCTS_Node(Position *pos, MCTS_Node *parent = nullptr);
        MCTS_Node *best_child(double Cp);

        inline bool is_leaf() { return children.empty(); }
        inline bool is_fully_expanded() { return children.size() == num_moves; }
    };

    MCTS_Node *select_node(MCTS_Node *node);

    MCTS_Node *expand_node(MCTS_Node *node);

    void back_propogate(MCTS_Node *node, score_t score);

public:

    score_t eval(Position *pos);

    Move best_move(Position &pos);

};

int rand_range(int max);

#endif // LEGEND_AI_H
