#include <iostream>
#include "position.h"
#include "legend_AI.h"

/**
 * Print out the list of possible moves to the user.
 * Returns the function the user chooses.
 */
Move prompt_move(Position & pos) {

    std::cout << "Move Choices:\n";
    std::vector<Move> all_moves = pos.get_all_legal_moves();
    int num_moves = all_moves.size();

    for (int i = 0; i < num_moves; i ++) {
        auto j = i + 1;
        std::cout << j / 10 << j % 10 << ": " << all_moves[i] << "\n";
    }

    int move_index;
    std::cout << "\nMove Index: ";
    while (true) {
        std::cin >> move_index;
        move_index--;

        if (0 <= move_index && move_index < num_moves) {
            break;
        }
        std::cout << "Chosen move index out of range.\n"
                  << "Please enter a valid move index: ";
    }

    std::cout << "Chosen move: " << all_moves[move_index] << "\n";

    return all_moves[move_index];
}

int main() {
    std::cout << "Starting a game of checkers.\n\n";

    Position game;

    LegendAI white_AI;
    LegendAI black_AI;

    while (true) {
        std::cout << game;
        std::cout << "\n\n";
        if (game.get_turn() == WHITE) {
            game = game.play_move(white_AI.best_move(game, 1));
        }
        else {
            // game = game.play_move(prompt_move(game));
            game = game.play_move(black_AI.best_move(game, 1));
        }
    }

    std::cout << "Thank you for playing checkers!\n";
    return 0;
}
