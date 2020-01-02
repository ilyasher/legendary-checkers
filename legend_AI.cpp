#include "legend_AI.h"

LegendAI::MCTS_Node::MCTS_Node(Position *pos, MCTS_Node *parent) {
    this->position = pos;
    this->parent = parent;

    // TODO: this gets calculated twice, fix to make faster!
    this->moves = pos->get_all_legal_moves();
    this->num_moves = moves.size();

    this->num_visits = 0;
    this->total_score = 0;
}

LegendAI::LegendAI() {
    game_tree = nullptr;
}

score_t LegendAI::eval(Position *pos) {

    // simple first attempt
    score_t white_score = 0;
    score_t black_score = 0;

    for (square_t square = 0; square < NUM_SQUARES; square ++) {
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

    // THIS IS SOMETHING THAT COULD BE CACHED !!!
    std::vector<Move> all_moves = node->moves;

    // TODO: Make this random.
    Move move = all_moves[node->children.size()];

    Position *new_position = new Position(node->position->play_move(move));
    if (!new_position) std::cout << "FATAL ERROR!\n";
    MCTS_Node *new_node = new MCTS_Node(new_position, node);
    if (!new_node) std::cout << "FATAL ERROR!\n";

    node->children.push_back(new_node);

    return new_node;
}

bool is_preferred(double a, double b, color_t turn) {
    // based on whose turn it is, returns whether a is preferred to b

    if (turn == WHITE) {
        return a > b;
    }
    return b > a;
}

LegendAI::MCTS_Node *LegendAI::MCTS_Node::best_child(double Cp) {

    double bound;
    MCTS_Node *best_node = nullptr;

    for (MCTS_Node *child : children) {
        double child_bound = (double) (child->total_score) / child->num_visits;

        // TODO: MAKE FASTER WITH TAYLOR, OR SOMEHOW:
        // TODO: store value of log(N)
        // TODO: make not ugly
        if (this->position->get_turn() == WHITE) {
            child_bound += Cp * std::sqrt(2 * std::log(num_visits) / child->num_visits);
        }
        else {
            child_bound -= Cp * std::sqrt(2 * std::log(num_visits) / child->num_visits);
        }

        // TODO: fix here
        if (!best_node || is_preferred(child_bound, bound, this->position->get_turn())) {
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

void LegendAI::delete_children(MCTS_Node *node) {
    for (auto child : node->children) {
        delete_children(child);
    }
    delete node->position;
    delete node;
}

LegendAI::MCTS_Node *LegendAI::MCTS_Node::find_explored_branch(Position *pos) {
    if (*this->position == *pos) {
        std::cout << "babus\n";
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
    // for (int i = 0; i < 4000; i ++) {
        MCTS_Node *node = select_node(game_tree);
        score_t score = eval(node->position);
        back_propogate(node, score);
        num_runs ++;
    }

    std::cout << "Performed " << num_runs << " runs\n";

    for (MCTS_Node *child : game_tree->children) {
        std::cout << "Child: " << child->num_moves << ", " << child->num_visits << ", "
                  << child->total_score << "\n";
    }

    Position *best_position = game_tree->best_child(0)->position;

    /* Free memory. */
    game_tree = game_tree->find_explored_branch(best_position);
    // std::cout << "e\n";
    // delete_children(game_tree);
    // std::cout << "f\n";

    for (Move move : pos.get_all_legal_moves()) {
        // TODO: save best child in variable
        if (pos.play_move(move) == *best_position) {
            return move;
        }
    }
    std::cout << "error\n";

    // TODO: throw error?
}

/* https://stackoverflow.com/questions/7560114/random-number-c-in-some-range */
int rand_range(int max) {

    std::random_device rd; // obtain a random number from hardware
    std::mt19937 eng(rd()); // seed the generator
    std::uniform_int_distribution<> distr(0, max); // define the range
    return distr(eng);
}
