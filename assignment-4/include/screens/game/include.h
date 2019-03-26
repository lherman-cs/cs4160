#include <cmath>
#include <tuple>
#include <unordered_map>
#include <vector>
#include "util/vector2f.h"

std::unordered_map<int, std::tuple<Vector2f, double>> dice_position = {
    {0, std::make_tuple(Vector2f(400, 650), 0)},
    {1, std::make_tuple(Vector2f(630, 120), (40 * M_PI) / 180)},
    {2, std::make_tuple(Vector2f(230, 240), (-40 * M_PI) / 180)},
    {3, std::make_tuple(Vector2f(220, 380), (70 * M_PI) / 180)},
    {4, std::make_tuple(Vector2f(730, 560), (-70 * M_PI) / 180)},
};