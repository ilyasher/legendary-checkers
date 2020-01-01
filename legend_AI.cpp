#include "legend_AI.h"

LegendAI::MCTS_Node::MCTS_Node(Position *pos, MCTS_Node *parent) {
    this->position = pos;
    this->parent = parent;

    // TODO: this gets calculated twice, fix to make faster!
    this->num_moves = pos->get_all_legal_moves().size();

    this->num_visits = 0;
    this->total_score = 0;
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


    // std::cout << "expanding node\n";

    // THIS IS SOMETHING THAT COULD BE CACHED !!!
    std::vector<Move> all_moves = node->position->get_all_legal_moves();

    // TODO: Make this random.
    Move move = all_moves[node->children.size()];

    // TODO: free these later
    Position *new_position = new Position(node->position->play_move(move));
    MCTS_Node *new_node = new MCTS_Node(new_position, node);

    node->children.push_back(new_node);

    return new_node;
}

LegendAI::MCTS_Node *LegendAI::MCTS_Node::best_child(double Cp) {
    double bound = 0;
    MCTS_Node *best_node;
    for (MCTS_Node *child : children) {
        // std::cout << "this happened once\n";
        double child_bound = (double) (child->total_score) / child->num_visits;
        // TODO: MAKE FASTER WITH TAYLOR, OR SOMEHOW:
        // TODO: store value of log(N)
        child_bound += Cp * std::sqrt(2 * std::log(num_visits) / child->num_visits);
        if (child_bound > bound) {
            bound = child_bound;
            best_node = child;
        }
        // std::cout << child << ", " << child_bound << "\n";
    }
    // std::cout << "Best child of " << this << " is " << best_node
              // << " with score " << bound << "\n";
    return best_node;
}

LegendAI::MCTS_Node *LegendAI::select_node(MCTS_Node *node) {

    // TODO: replace with while(game is not over)
    while (true) {
        if (!node->is_fully_expanded()) {
            return expand_node(node);
        }
        // std::cout << node->children.size() << " " << node->num_moves << "\n";
        // std::cout << "about to try the best child\n";
        node = node->best_child(1.4142);
    }
    return node;
}

void LegendAI::back_propogate(LegendAI::MCTS_Node *node, score_t score) {
    while (node) {
        node->num_visits ++;
        node->total_score += score;

        // TODO: CHECK THIS.
        // score *= -1;

        node = node->parent;
    }
}

Move LegendAI::best_move(Position &pos) {

    MCTS_Node game_tree = MCTS_Node(&pos);

    auto start_time = std::clock();

    int num_runs = 0;

    while (( std::clock() - start_time ) / (double) CLOCKS_PER_SEC < 0.01) {
    // for (int i = 0; i < 4000; i ++) {
        MCTS_Node *node = select_node(&game_tree);
        // std::cout << node->position << "\n";
        score_t score = eval(node->position);
        back_propogate(node, score);
        num_runs ++;
    }

    std::cout << "Performed " << num_runs << " runs\n";

    // std::cout << game_tree.children.size() << "\n";
    // std::cout << game_tree.num_moves << "\n";
    for (MCTS_Node *child : game_tree.children) {
        // std::cout << "Child: " << child->num_moves << ", " << child->num_visits << ", "
                  // << child->total_score << "\n";
    }

    Position best_position = *(game_tree.best_child(0)->position);

    // std::cout << "Made it here ??\n";


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
