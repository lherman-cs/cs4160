#pragma once

#include <SDL2/SDL.h>

class Screen {
 public:
  virtual ~Screen();
  virtual void onKeyDown(const Uint8* const keystate) = 0;
  virtual void draw() const = 0;
  virtual void update(Uint32 ticks) = 0;

 protected:
  Screen();
};