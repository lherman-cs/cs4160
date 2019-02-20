#include "renderer/renderer.h"
#include "renderer/color.h"

Renderer::Renderer(SDL_Renderer *renderer, int x, int y, int w, int h)
    : renderer(renderer), x(x), y(y), w(w), h(h) {
  // surpressed unused w and h, with do nothing trick
  (void)this->w;
  (void)this->h;
}

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
  SDL_Rect rect = {this->x + x, this->y + y, w, h};

  // TODO! error handler
  SDL_RenderDrawRect(renderer, &rect);
}

void Renderer::fillRect(int x, int y, int w, int h) const {
  SDL_Rect rect = {this->x + x, this->y + y, w, h};

  // TODO! error handler
  SDL_RenderFillRect(renderer, &rect);
}

void Renderer::setDrawColor(Color c, uint8_t a) const {
  SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, a);
}