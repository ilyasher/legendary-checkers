#include "legend_AI.h"

LegendAI::MCTS_Node::MCTS_Node(Position *pos, MCTS_Node *parent) {
    this->position = pos;
    this->parent = parent;

    this->moves = pos->get_all_legal_moves();
    this->num_moves = moves.size();

    this->num_visits = 0;
    this->total_score = 0;
    this->visited_move_indices = 0;
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

    /* Choose a random move from the list of unplayed moves. */
    int num_unplayed_moves = 0;
    for (int i = 0; i < node->num_moves; i ++) {
        if (square_index_to_bitboard(i) & ~node->visited_move_indices) {
            num_unplayed_moves ++;
        }
    }
    int random_index = random_engine.rand_range(num_unplayed_moves);
    int move_index;
    int unplayed_move_number = 0;
    for (move_index = 0; move_index < node->num_moves; move_index ++) {
        if (square_index_to_bitboard(move_index) & ~node->visited_move_indices) {
            if (unplayed_move_number == random_index) {
                break;
            }
            unplayed_move_number ++;
        }
    }

    /* Record move as played */;
    node->visited_move_indices |= square_index_to_bitboard(move_index);

    Move move = node->moves[move_index];

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
        std::cout << "FOUND!!\n";
        this->parent = nullptr;
        return this;
    }
    MCTS_Node *return_value = nullptr;
    for (auto child : this->children) {
        MCTS_Node *node = child->find_explored_branch(pos);
        if (node && !return_value) {
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
        if (!game_tree) {
            std::cout << "could not find subtree" << "\n";
            game_tree = new MCTS_Node(pos_copy);
        }
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
        std::cout << "Child: " << child->num_moves << ", " << child->num_visits << ", "
                  << (child->total_score / child->num_moves) << "\n";
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
