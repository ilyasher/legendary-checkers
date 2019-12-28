#include <iostream>
#include "position.h"

Move prompt_move(Position & pos) {

    std::cout << "Move Choices:\n";
    std::vector<Move> all_moves = pos.get_all_legal_moves();
    // TODO: What if you have no moves??
    for (int i = 0; i < all_moves.size(); i ++) {
        auto j = i + 1;
        std::cout << j / 10 << j % 10 << ": " << all_moves[i] << "\n";
    }
    std::cout << "\nMove Index: ";

    int move_index;
    std::cin >> move_index;
    move_index--;
    // TODO: Make sure input is valid
    std::cout << "Chosen move index: " << move_index << "\n";
    std::cout << "Chosen move: " << all_moves[move_index] << "\n";

    return all_moves[move_index];
}

int main() {
    std::cout << "Starting a game of checkers.\n\n";

    Position game;

    while (true) {
        std::cout << game;
        std::cout << "\n";
        game = game.play_move(prompt_move(game));
    }

    std::cout << "Thank you for playing checkers!\n";
    return 0;
}
