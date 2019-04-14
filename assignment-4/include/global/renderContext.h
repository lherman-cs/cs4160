#pragma once
#include <SDL2/SDL.h>
#include <iostream>
#include "imageFactory.h"

class Global;

class RenderContext {
 public:
  ~RenderContext();
  SDL_Window* getWindow() const { return window; }
  SDL_Renderer* getRenderer() const { return renderer; }

  RenderContext(const RenderContext&) = delete;
  RenderContext& operator=(const RenderContext&) = delete;

 private:
  friend class Global;
  Gamedata& gamedata;
  SDL_Window* window;
  SDL_Renderer* renderer;

  SDL_Window* initWindow();
  SDL_Renderer* initRenderer();
  RenderContext(Gamedata& gamedata);
};