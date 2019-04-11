#include <cmath>
#include <utility>
#include "util/vector2f.h"

const std::pair<Vector2f, double> dicePos[] = {
    {Vector2f(400, 650), 0},
    {Vector2f(630, 120), (40 * M_PI) / 180},
    {Vector2f(230, 240), (-40 * M_PI) / 180},
    {Vector2f(220, 380), (72 * M_PI) / 180},
    {Vector2f(730, 560), (-72 * M_PI) / 180}};