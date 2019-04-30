#pragma once
#include <cmath>
#include <utility>
#include "util/vector2f.h"

const std::pair<Vector2f, double> dicePos[] = {
    {Vector2f(410, 630), 0},
    {Vector2f(230, 390), (67 * M_PI) / 180},
    {Vector2f(270, 270), (-50 * M_PI) / 180},
    {Vector2f(630, 130), (50 * M_PI) / 180},
    {Vector2f(700, 560), (-67 * M_PI) / 180}};