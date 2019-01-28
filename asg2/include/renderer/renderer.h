#pragma once

#include <SDL2/SDL.h>
#include "renderer/color.h"

class Renderer {
 public:
  Renderer(SDL_Renderer *renderer, int x, int y, int w, int h);
  void drawPoint(int x, int y) const;
  void drawLine(int x1, int y1, int x2, int y2) const;
  void drawRect(int x, int y, int w, int h) const;
  void fillRect(int x, int y, int w, int h) const;
  void setDrawColor(Color c, uint8_t a) const;

 private:
  SDL_Renderer *renderer;

  // these define the bounding box, so that the user will not be able
  // to go beyond its box. This should change all the times, depending
  // on the the shape that is currently being drawn
  const int x, y, w, h;
};