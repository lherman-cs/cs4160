#pragma once

#include <SDL2/SDL.h>
#include <string>
#include "renderer/shape.h"

class Window {
 public:
  Window(int width, int height, const std::string title);
  Window(const Window &other) = delete;           // maybe?
  Window &operator=(const Window &rhs) = delete;  // maybe?
  ~Window();
  void loop(std::shared_ptr<const Shape> root);
  void save(std::shared_ptr<const Shape> root, const std::string name);

 private:
  SDL_Renderer *renderer;
  SDL_Window *window;
  const int width, height;
  const std::string title;

  void draw(std::shared_ptr<const Shape> root, int x, int y);
};