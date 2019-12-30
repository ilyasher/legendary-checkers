#include "legend_AI.h"

LegendAI::PositionTreeNode::PositionTreeNode(const Position *pos) {
    PositionTreeNode(pos, nullptr);
}

LegendAI::PositionTreeNode::PositionTreeNode(const Position *pos, const PositionTreeNode *parent) {
    this->position = pos;
    this->parent = parent;
}

score_t LegendAI::eval(const Position &pos) {

    // simple first attempt
    score_t score = 0;

    for (square_t square = 0; square < NUM_SQUARES; square ++) {
        if (square_is_on(pos.white_men(), square)) {
            score += 100;
        }
        else if (square_is_on(pos.white_kings(), square)) {
            score += 300;
        }
        else if (square_is_on(pos.black_men(), square)) {
            score += -100;
        }
        else if (square_is_on(pos.black_kings(), square)) {
            score += -300;
        }
    }

    return score;
}

LegendAI::Move best_move(const Position &pos) {

    game_tree = PositionTreeNode(&pos);

    // later, replace with `for 1 second`
    for (int i = 0; i < 100; i ++) {
        
    }

}
