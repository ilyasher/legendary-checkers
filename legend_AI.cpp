#include "legend_AI.h"

LegendAI::PositionTreeNode::PositionTreeNode(const Position *pos) {
    PositionTreeNode(pos, nullptr);
}

LegendAI::PositionTreeNode::PositionTreeNode(const Position *pos, const PositionTreeNode *parent) {
    this->position = pos;
    this->parent = parent;
    this->num_moves = pos->get_all_legal_moves().size();
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

LegendAI::PositionTreeNode *LegendAI::expand_node(PositionTreeNode *node) {

    // THIS IS SOMETHING THAT SHOULD BE CACHED !!!
    std::vector<Move> all_moves = node->position->get_all_legal_moves();

    // TODO: Make this random.
    Move move = all_moves[node->children.size()];

    // TODO: free these later
    Position *new_position = new Position(node->position->play_move(move));
    PositionTreeNode *new_node = new PositionTreeNode(new_position, node);

    return new_node;
}

LegendAI::PositionTreeNode *LegendAI::select_node(PositionTreeNode *node) {
    while (!node->is_leaf()) {
        if (!node->is_fully_expanded()) {
            return expand_node(node);
        }
        // node = best_child();
    }

}

Move LegendAI::best_move(const Position &pos) {

    game_tree = PositionTreeNode(&pos);

    // TODO: replace with `for 1 second`
    for (int i = 0; i < 100; i ++) {
        PositionTreeNode *node = select_node(&game_tree);
    }
}

/* https://stackoverflow.com/questions/7560114/random-number-c-in-some-range */
int rand_range(int max) {
    std::random_device rd; // obtain a random number from hardware
    std::mt19937 eng(rd()); // seed the generator
    std::uniform_int_distribution<> distr(0, max); // define the range
    return distr(eng);
}
