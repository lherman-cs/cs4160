#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include "frameGen/frameGenerator.h"
#include "shapes/circle.h"
#include "shapes/container.h"
#include "shapes/line.h"
#include "shapes/rect.h"
#include "shapes/triangle.h"
#include "window/window.h"

const std::string TITLE = "Ian Burch's Bullseyes";
const std::string NAME = "malloy";

const int WIDTH = 400;
const int HEIGHT = 400;

int main(void) {
  Window w(WIDTH, HEIGHT);

  // auto c2 = CircleBuilder(100, 100, 80)
  //               .setFillMode(Fill::SOLID)
  //               .setFillColor(Colors::WHITE)
  //               .build();

  // auto c1 = CircleBuilder(400 - 100, 100, 80)
  //               .setFillMode(Fill::NONE)
  //               .setBorderColor(Colors::WHITE)
  //               .build();

  // auto line = LineBuilder(0, 0, 640,
  // 480).setFillColor(Colors::GOPHER).build();

  // auto paper = RectBuilder(50, 50, 400, 200)
  //                  .setFillColor(Colors::WHITE)
  //                  .add(c1)
  //                  .add(c2)
  //                  .build();

  auto tri = TriangleBuilder(0, 400, 200, 0, 400, 200).build();

  Container root(0, 0, WIDTH, HEIGHT, {tri});
  w.loop(root);
}
