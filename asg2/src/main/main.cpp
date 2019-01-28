#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include "frameGen/frameGenerator.h"
#include "shapes/circle.h"
#include "shapes/container.h"
#include "window/window.h"

const std::string TITLE = "Ian Burch's Bullseyes";
const std::string NAME = "malloy";

const int WIDTH = 640;
const int HEIGHT = 480;

int main(void) {
  Window w(WIDTH, HEIGHT);

  auto c2 = CircleBuilder(100, 100, 30)
                .setFillMode(Fill::SOLID)
                .setFillColor(Colors::WHITE)
                .build();

  auto c1 = CircleBuilder(320, 240, 100)
                .setFillMode(Fill::NONE)
                .setBorderColor(Colors::WHITE)
                .setChildren({c2})
                .build();

  Container root(0, 0, WIDTH, HEIGHT, {c1});
  w.loop(root);
}
