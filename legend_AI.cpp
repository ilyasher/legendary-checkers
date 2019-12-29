#include "legend_AI.cpp"

score_t LegendAI::eval(const Position &pos) {

    // simple first attempt
    score_t score = 0;

    for (square_t i = 0; i < NUM_SQUARES; i ++) {
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
