#pragma once
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
      const Value& l = {0, 1}, const Value& c = {1, 1});
  Bet(const Bet&) = delete;
  Bet& operator=(const Bet&) = delete;
  void draw() const;
  void setSelectable(bool);
  void select(Type);
  Bet& increment(Type);
  Bet& decrement(Type);
  int get(Type) const;

 private:
  const GameScreen* const game;
  Vector2f position;
  bool dieSelected{false};
  SDL_Color normalColor{52, 44, 42, 255};
  SDL_Color hoverColor{255, 255, 0, 255};
  SDL_Color* textColor = &hoverColor;
  Value last;
  Value current;
  const int numFaces{6};
  IoMod menuWriter{60};
  Die die{Vector2f(950, 50), Die::State::visible, current.face - 1};

  bool validate();
};