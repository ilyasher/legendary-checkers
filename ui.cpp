#include <iostream>
#include "position.h"

Move prompt_move(Position & pos) {
    std::vector<Move> all_moves = pos.get_all_legal_moves();
    for (Move move : all_moves) {
        std::cout << move.from << ", " << move.to << "\n";
    }
}

int main() {
    std::cout << "Starting a game of checkers.\n";

    Position game;

    std::cout << "*---*---*---*---*---*---*---*---*\n";
    for (square_t square = 0; square < NUM_SQUARES; square ++) {

        std::cout << "| ";

        char to_print = ' ';
        if (square_is_on(game.white_men(), square))
            to_print = 'o';
        if (square_is_on(game.black_men(), square))
            to_print = 'x';
        if (square_is_on(game.white_kings(), square))
            to_print = 'O';
        if (square_is_on(game.black_kings(), square))
            to_print = 'X';

        std::cout << to_print;
        std::cout << " ";

        if ((square + 1) % (NUM_SQUARES / ROW) == 0) {
            std::cout << "|\n";
            std::cout << "*---*---*---*---*---*---*---*---*\n";
        }
    }
    prompt_move(game);

    std::cout << "Thank you for playing checkers!\n";
    return 0;
}
