#include "renderer/renderer.h"

Renderer::Renderer(SDL_Renderer *renderer, int x, int y, int w, int h)
    : renderer(renderer), x(x), y(y), w(w), h(h) {}

void Renderer::drawPoint(int x, int y) const {
  // TODO! error handler
  SDL_RenderDrawPoint(renderer, this->x + x, this->y + y);
}

void Renderer::drawLine(int x1, int y1, int x2, int y2) const {
  // TODO! error handler
  SDL_RenderDrawLine(renderer, this->x + x1, this->y + y1, this->x + x2,
                     this->y + y2);
}

void Renderer::drawRect(int x, int y, int w, int h) const {
  SDL_Rect rect = {this->x + y, this->y + y, w, h};

  // TODO! error handler
  SDL_RenderDrawRect(renderer, &rect);
}

void Renderer::setDrawColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a) const {
  SDL_SetRenderDrawColor(renderer, r, g, b, a);
}