#pragma once
#include <SDL2/SDL.h>
#include <iostream>
#include <string>
#include "util/vector2f.h"

class Image {
 public:
  Image(SDL_Surface*);

  void regenerateTexture();
  void draw(const Vector2f& pos) const;
  void draw(int x, int y) const;
  void draw(int x, int y, float scale) const;
  void draw(int sx, int sy, int dx, int dy) const;

  inline int getWidth() const { return surface->w; }
  inline int getHeight() const { return surface->h; }
  SDL_Surface* getSurface() const { return surface; }
  Image* crop(SDL_Rect) const;

 private:
  SDL_Renderer* renderer;
  SDL_Surface* surface;
  SDL_Texture* texture;
  SDL_Rect view;
};
