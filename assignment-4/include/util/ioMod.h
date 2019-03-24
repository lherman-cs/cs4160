#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <string>

class IoMod {
 public:
  IoMod(int size, SDL_Color textColor);
  ~IoMod();
  void writeText(const std::string&, int, int) const;
  IoMod(const IoMod&) = delete;
  IoMod& operator=(const IoMod&) = delete;

 private:
  int init;
  SDL_Renderer* renderer;
  TTF_Font* font;
  SDL_Color textColor;
};