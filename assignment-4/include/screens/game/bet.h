// Bet class, a shared drawable object which is responsible for maintaining
// bet validity and drawing the bet, managing highlighted states

#pragma once
#include <SDL2/SDL.h>  // for Uint32
#include "core/interface.h"
#include "util/vector2f.h"

class Bet : public Drawable {
 public:
  Bet(const Vector2f& pos = Vector2f(0, 0), int gap = 0, int quan = 0,
      int face = 1);
  void draw() const;
  void update(Uint32 ticks);
  Bet& incQuan(int);
  Bet& decQuan(int);
  Bet& incFace(int);
  Bet& decFace(int);

  static int getSize();

 private:
  int gap = 0;
  Vector2f position = Vector2f(0, 0);
};