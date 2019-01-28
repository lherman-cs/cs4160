#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include "basicShapes/circle.h"
#include "basicShapes/container.h"
#include "frameGen/frameGenerator.h"
#include "window/window.h"

const std::string TITLE = "Ian Burch's Bullseyes";
const std::string NAME = "malloy";

const int WIDTH = 640;
const int HEIGHT = 480;

int main(void) {
  Window w(WIDTH, HEIGHT);

  auto c1 = CircleBuilder(320, 240, 30)
                .setFillMode(Fill::SOLID)
                .setFillColor(Colors::RED)
                .build();

  Container root(0, 0, WIDTH, HEIGHT, {c1});
  w.loop(root);
}
