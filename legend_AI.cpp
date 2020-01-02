#include "legend_AI.h"

LegendAI::MCTS_Node::MCTS_Node(Position *pos, MCTS_Node *parent) {
    this->position = pos;
    this->parent = parent;

    this->moves = pos->get_all_legal_moves();
    this->num_moves = moves.size();

    this->num_visits = 0;
    this->total_score = 0;
}

LegendAI::LegendAI() {
    game_tree = nullptr;
}

score_t LegendAI::eval(Position *pos) {

    // TODO: make stronger
    score_t white_score = 0;
    score_t black_score = 0;

    square_t square = 1;
    for (uint8_t i = 0; i < NUM_SQUARES; i ++) {
        if (square_is_on(pos->white_men(), square)) {
            white_score += 100;
        }
        else if (square_is_on(pos->white_kings(), square)) {
            white_score += 300;
        }
        else if (square_is_on(pos->black_men(), square)) {
            black_score += 100;
        }
        else if (square_is_on(pos->black_kings(), square)) {
            black_score += 300;
        }
        square <<= 1;
    }

    if (black_score == 0) {
        return WHITE_WIN;
    }
    if (white_score == 0) {
        return BLACK_WIN;
    }

    return white_score - black_score;
}

LegendAI::MCTS_Node *LegendAI::expand_node(MCTS_Node *node) {

    std::vector<Move> all_moves = node->moves;

    // TODO: Make this random.
    Move move = all_moves[node->children.size()];

    Position *new_position = new Position(node->position->play_move(move));
    MCTS_Node *new_node = new MCTS_Node(new_position, node);

    node->children.push_back(new_node);

    return new_node;
}

/* Based on whose turn it is, returns whether a is preferred to b. */
inline bool is_preferred(double a, double b, color_t turn) {
    if (turn == WHITE) {
        return a > b;
    }
    return b > a;
}

LegendAI::MCTS_Node *LegendAI::MCTS_Node::best_child(double Cp) {

    double bound;
    MCTS_Node *best_node = nullptr;

    double log_num_visits = std::log(num_visits);

    for (MCTS_Node *child : children) {
        double child_bound = (double) (child->total_score) / child->num_visits;

        // TODO: MAKE FASTER WITH TAYLOR, OR SOMEHOW:
        color_t child_turn = this->position->get_turn();

        double exploration_value = Cp * std::sqrt(2 * log_num_visits / child->num_visits);
        if (child_turn == BLACK) {
            exploration_value *= -1;
        }
        child_bound += exploration_value;

        if (!best_node || is_preferred(child_bound, bound, child_turn)) {
            bound = child_bound;
            best_node = child;
        }
    }

    if (Cp == 0) {
        std::cout << "Best child is " << best_node
                  << " with score " << bound << "\n";
    }

    return best_node;
}

LegendAI::MCTS_Node *LegendAI::select_node(MCTS_Node *node) {

    // TODO: replace with while(game is not over)
    while (true) {
        if (!node->is_fully_expanded()) {
            return expand_node(node);
        }
        node = node->best_child(420.69);
    }

    return node;
}

void LegendAI::back_propogate(LegendAI::MCTS_Node *node, score_t score) {
    while (node) {
        node->num_visits ++;
        node->total_score += score;
        node = node->parent;
    }
}

LegendAI::MCTS_Node *LegendAI::MCTS_Node::find_explored_branch(Position *pos) {
    if (*this->position == *pos) {
        this->parent = nullptr;
        return this;
    }
    MCTS_Node *return_value = nullptr;
    for (auto child : this->children) {
        MCTS_Node *node = child->find_explored_branch(pos);
        if (node) {
            return_value = node;
        }
    }
    delete this->position;
    delete this;
    return return_value;
}

Move LegendAI::best_move(Position &pos, double time) {
    Position *pos_copy = new Position(pos);
    if (!game_tree) {
        game_tree = new MCTS_Node(pos_copy);
    }
    else {
        game_tree = game_tree->find_explored_branch(pos_copy);
    }

    auto start_time = std::clock();

    int num_runs = 0;

    while (( std::clock() - start_time ) / (double) CLOCKS_PER_SEC < time) {
        MCTS_Node *node = select_node(game_tree);
        score_t score = eval(node->position);
        back_propogate(node, score);
        num_runs ++;
    }

    std::cout << "Performed " << num_runs << " runs\n";

    for (MCTS_Node *child : game_tree->children) {
        std::cout << "Child: " << child->num_moves << ", " << child->num_visits << ", ";
                  // << child->total_score << "\n";
    }

    MCTS_Node *best_child_node = game_tree->best_child(0);
    Position *best_position = best_child_node->position;
    std::cout << "Num visits: " << best_child_node->num_visits << "\n";

    /* Free memory. */
    game_tree = game_tree->find_explored_branch(best_position);

    for (Move move : pos.get_all_legal_moves()) {
        if (pos.play_move(move) == *best_position) {
            return move;
        }
    }
    throw std::runtime_error("Something's horribly wrong.\n");
}

/* https://stackoverflow.com/questions/7560114/random-number-c-in-some-range */
int rand_range(int max) {
    std::random_device rd; // obtain a random number from hardware
    std::mt19937 eng(rd()); // seed the generator
    std::uniform_int_distribution<> distr(0, max); // define the range
    return distr(eng);
}
