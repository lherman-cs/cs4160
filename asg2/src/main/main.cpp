#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include "frameGen/frameGenerator.h"
#include "main/gopher.h"
#include "window/window.h"

const std::string TITLE = "Ian Burch's Bullseyes";
const std::string NAME = "malloy";

const int WIDTH = 800;
const int HEIGHT = 600;

int main(void) {
  Window w(WIDTH, HEIGHT, "Gophers!!!");

  auto gophers = ContainerBuilder(0, 0, WIDTH, HEIGHT);

  int nGophers = 20;
  int gopherHeight = 100;
  int start = 0;
  int offset = 0;

  for (int y = 200; y <= 500; y += 50) {
    for (int n = start; n < nGophers; n++) {
      gophers.add(buildGopher(offset + n * 75, y, gopherHeight));
    }

    if (start == 0) {
      start = -1;
      offset = 50;
    } else {
      start = 0;
      offset = 0;
    }
  }

  gophers.add(buildGopher(400, 100, 700));
  // w.loop(gophers.build());
  w.save(gophers.build(), "Lukas Herman");
}
