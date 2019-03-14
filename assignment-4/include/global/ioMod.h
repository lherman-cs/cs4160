#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <string>

class IoMod {
 public:
  static IoMod& getInstance();
  ~IoMod();
  SDL_Texture* readTexture(const std::string& filename);
  SDL_Surface* readSurface(const std::string& filename);
  void writeText(const std::string&, int, int) const;
  void writeText(const std::string& msg, int x, int y, SDL_Color color) const;
  SDL_Renderer* getRenderer() const { return renderer; }
  IoMod(const IoMod&) = delete;
  IoMod& operator=(const IoMod&) = delete;

 private:
  int init;
  SDL_Renderer* renderer;
  TTF_Font* font;
  SDL_Color textColor;
  IoMod();
};