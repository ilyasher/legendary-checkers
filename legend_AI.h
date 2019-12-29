#ifndef LEGEND_AI_H
#define LEGEND_AI_H

#include "position.h"

using score_t = int32_t;

class LegendAI {

    // ideas: cache game states

public:

    score_t eval(const Position &pos);

    Move best_move(const Position &pos);

};

#endif // LEGEND_AI_H
