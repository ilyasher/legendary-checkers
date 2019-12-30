#ifndef LEGEND_AI_H
#define LEGEND_AI_H

#include <random>
#include "position.h"

using score_t = int32_t;

class LegendAI {

    // ideas: cache game states

    struct PositionTreeNode {
        const PositionTreeNode *parent;
        const Position *position;
        int num_moves;

        std::vector<PositionTreeNode *> children;

        PositionTreeNode(const Position *pos);
        PositionTreeNode(const Position *pos, const PositionTreeNode *parent);

        inline bool is_leaf() { return children.empty(); }
        inline bool is_fully_expanded() { return children.size() == num_moves; }
    };

    PositionTreeNode game_tree;

    PositionTreeNode *select_node(PositionTreeNode *node);

    PositionTreeNode *expand_node(PositionTreeNode *node);


public:

    score_t eval(const Position &pos);

    Move best_move(const Position &pos);

};

int rand_range(int max);

#endif // LEGEND_AI_H
