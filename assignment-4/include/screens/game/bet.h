#pragma once
#include "core/interface.h"  // for drawable
#include "die.h"             // for Die rendering
#include "net/state.h"       // for state
#include "util/ioMod.h"      // for drawing
#include "util/vector2f.h"   // for vector

class State;

class Bet : public Drawable {
 public:
  enum Type { Quantity, Face };
  struct Value {
    int quantity;
    int face;
  };

  Bet(const State* const s, const Vector2f& pos = Vector2f(0, 0),
      const Value& l = {0, 1}, const Value& c = {1, 1});
  Bet(const Bet&) = delete;
  Bet& operator=(const Bet&) = delete;
  void draw() const;
  void setSelectable(bool);
  void select(Type);
  void submit();
  Bet& increment(Type);
  Bet& decrement(Type);
  Value getLast() const;
  Value getCurr() const;
  void reset();

 protected:
  void set(const Value&);

 private:
  friend State;

  // Generic Data
  const State* state;
  static const int numFaces{6};

  // Drawing Data
  Vector2f position;
  bool dieSelected{false};

  // Bet Data
  Value last{};
  Value current{};
  Die die{Vector2f(950, 50), Die::State::visible, current.face - 1};

  // Drawing
  SDL_Color normalColor{52, 44, 42, 255};
  SDL_Color hoverColor{255, 255, 0, 255};
  SDL_Color* textColor = &hoverColor;
  IoMod menuWriter{60};

  // Validate bet
  bool validate();
};