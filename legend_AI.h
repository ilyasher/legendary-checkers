#ifndef LEGEND_AI_H
#define LEGEND_AI_H

#include <random>
#include <cmath>
#include <iostream>
#include "position.h"

using score_t = int32_t;

class LegendAI {

    // ideas: cache game states

    struct PositionTreeNode {
        PositionTreeNode *parent;
        Position *position;
        int num_moves;

        int num_visits;
        int total_score;

        std::vector<PositionTreeNode *> children;

        PositionTreeNode(Position *pos);
        PositionTreeNode(Position *pos, PositionTreeNode *parent);
        PositionTreeNode *best_child(double Cp);

        inline bool is_leaf() { return children.empty(); }
        inline bool is_fully_expanded() { return children.size() == num_moves; }
    };

    // PositionTreeNode game_tree;

    PositionTreeNode *select_node(PositionTreeNode *node);

    PositionTreeNode *expand_node(PositionTreeNode *node);

    void back_propogate(PositionTreeNode *node, score_t score);

public:

    score_t eval(Position *pos);

    Move best_move(Position &pos);

};

int rand_range(int max);

#endif // LEGEND_AI_H
