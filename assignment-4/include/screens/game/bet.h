#pragma once
#include <SDL2/SDL.h>           // for Uint32
#include "core/interface.h"     // for drawable
#include "die.h"                // for Die rendering
#include "screens/game/game.h"  // for GameScreen
#include "util/ioMod.h"         // for drawing
#include "util/vector2f.h"      // for vector

class GameScreen;

class Bet : public Drawable {
 public:
  enum Type { Quantity, Face };
  struct Value {
    int quantity;
    int face;
  };

  Bet(const GameScreen* const, const Vector2f& pos = Vector2f(0, 0),
      int gap = 0, const Value& l = {0, 1}, const Value& c = {1, 1});
  Bet(const Bet&) = delete;
  Bet& operator=(const Bet&) = delete;
  void draw() const;
  void setSelectable(bool);
  Bet& increment(Type);
  Bet& decrement(Type);
  int getQuantity() const;
  int getType() const;

 private:
  const GameScreen* const game;
  Vector2f position;
  int gap;
  bool selectable{false};
  Value last;
  Value current;
  const int numFaces{6};
  IoMod menuWriter{60};
  // Die die{Vector2f(950, 50), Die::State::visible, current.face - 1};

  void validate();
};