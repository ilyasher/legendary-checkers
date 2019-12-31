#include "legend_AI.h"

LegendAI::PositionTreeNode::PositionTreeNode(Position *pos) {
    // PositionTreeNode(pos, nullptr);
    this->position = pos;
    this->parent = nullptr;
    this->num_moves = pos->get_all_legal_moves().size();
    this->num_visits = 0;
    this->total_score = 0;
}

LegendAI::PositionTreeNode::PositionTreeNode(Position *pos, PositionTreeNode *parent) {
    this->position = pos;
    this->parent = parent;
    this->num_moves = pos->get_all_legal_moves().size();
    this->num_visits = 0;
    this->total_score = 0;
}

score_t LegendAI::eval(Position *pos) {

    // simple first attempt
    score_t score = 0;

    for (square_t square = 0; square < NUM_SQUARES; square ++) {
        if (square_is_on(pos->white_men(), square)) {
            score += 100;
        }
        else if (square_is_on(pos->white_kings(), square)) {
            score += 300;
        }
        else if (square_is_on(pos->black_men(), square)) {
            score += -100;
        }
        else if (square_is_on(pos->black_kings(), square)) {
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

    node->children.push_back(new_node);

    return new_node;
}

LegendAI::PositionTreeNode *LegendAI::PositionTreeNode::best_child(double Cp) {
    double bound = 0;
    PositionTreeNode *best_node;
    for (PositionTreeNode *child : children) {
        std::cout << "this happened once\n";
        double child_bound = (double) (child->total_score) / child->num_visits;
        // TODO: MAKE FASTER WITH TAYLOR, OR SOMEHOW:
        // TODO: store value of log(N)
        child_bound += Cp * std::sqrt(2 * std::log(num_visits) / child->num_visits);
        if (child_bound > bound) {
            bound = child_bound;
            best_node = child;
        }
        std::cout << child << ", " << child_bound << "\n";
    }
    std::cout << "Best child of " << this << " is " << best_node << "\n";
    return best_node;
}

LegendAI::PositionTreeNode *LegendAI::select_node(PositionTreeNode *node) {
    do {
        if (!node->is_fully_expanded()) {
            return expand_node(node);
        }
        std::cout << "about to try the best child\n";
        node = node->best_child(1.4142);
    } while (!node->is_leaf());
    return node;
}

void LegendAI::back_propogate(LegendAI::PositionTreeNode *node, score_t score) {
    while (node) {
        node->num_visits ++;
        node->total_score += score;

        // TODO: CHECK THIS.
        score *= -1;

        node = node->parent;
    }
}

Move LegendAI::best_move(Position &pos) {

    PositionTreeNode game_tree = PositionTreeNode(&pos);

    // TODO: replace with `for 1 second`
    for (int i = 0; i < 10000; i ++) {
        PositionTreeNode *node = select_node(&game_tree);
        std::cout << node->position << "\n";
        score_t score = eval(node->position);
        back_propogate(node, score);
    }

    std::cout << game_tree.children.size() << "\n";
    std::cout << game_tree.num_moves << "\n";
    for (PositionTreeNode *child : game_tree.children) {
        std::cout << "Child: " << child->num_moves << ", " << child->num_visits << ", "
                  << child->total_score << "\n";
    }

    Position best_position = *(game_tree.best_child(0)->position);

    std::cout << "Made it here ??\n";


    for (Move move : pos.get_all_legal_moves()) {
        // TODO: save best child in variable
        // std::cout << "Considering move " << move << "\n";
        if (pos.play_move(move) == best_position) {
            return move;
        }
    }

    // TODO: throw error?
}

/* https://stackoverflow.com/questions/7560114/random-number-c-in-some-range */
int rand_range(int max) {

    std::random_device rd; // obtain a random number from hardware
    std::mt19937 eng(rd()); // seed the generator
    std::uniform_int_distribution<> distr(0, max); // define the range
    return distr(eng);
}
