#pragma once

#include <stdint.h>

struct Color {
  uint8_t r, g, b;
};

namespace Colors {

const Color RED = {255, 0, 0};
const Color GREEN = {0, 255, 0};
const Color BLUE = {0, 0, 255};
const Color BLACK = {0, 0, 0};
const Color WHITE = {255, 255, 255};

}  // namespace Colors
