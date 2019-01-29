#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include "frameGen/frameGenerator.h"
#include "shapes/circle.h"
#include "shapes/container.h"
#include "shapes/ellipse.h"
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

  auto el =
      EllipseBuilder(200, 200, 50, 80).setBorderColor(Colors::RED).build();

  auto root = ContainerBuilder(0, 0, WIDTH, HEIGHT).add(el).build();
  w.loop(root);
}
