#ifndef LEGEND_AI_H
#define LEGEND_AI_H

#include "position.h"

using score_t = int32_t;

class LegendAI {

    // ideas: cache game states

    struct PositionTreeNode {
        const PositionTreeNode *parent;
        const Position *position;
        std::vector<PositionTreeNode *> children;

        PositionTreeNode(const Position *pos);
        PositionTreeNode(const Position *pos, const PositionTreeNode *parent);
    };

    PositionTreeNode game_tree;



public:

    score_t eval(const Position &pos);

    Move best_move(const Position &pos);

};

#endif // LEGEND_AI_H
