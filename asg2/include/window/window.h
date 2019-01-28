#pragma once

#include <SDL2/SDL.h>
#include "renderer/shape.h"

class Window {
 public:
  Window(int width, int height);
  Window(const Window &other) = delete;           // maybe?
  Window &operator=(const Window &rhs) = delete;  // maybe?
  ~Window();
  void loop(const Shape &root);

 private:
  SDL_Renderer *renderer;
  SDL_Window *window;
  const int width, height;

  void draw(const Shape &root, int x, int y);
};