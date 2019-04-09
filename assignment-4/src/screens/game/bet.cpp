#include "screens/game/bet.h"
#include "screens/game/die.h"

Bet::Bet(const GameScreen* const g, const Vector2f& pos, int ga,
         const Value& val)
    : game(g), position(pos), gap(ga), last(val), current(val) {}

void Bet::draw() const {
  SDL_Color normalColor = SDL_Color({52, 44, 42, 255});
  SDL_Color hoverColor = SDL_Color({255, 255, 0, 255});

  menuWriter.writeText(std::to_string(current.quantity), 30, 825,
                       !game->getSelected() ? hoverColor : normalColor);

  Die d = Die(Vector2f(950, 50), Die::State::visible, current.face - 1);
  game->getSelected() ? d.select().draw() : d.deselect().draw();
}

Bet& Bet::increment(Type t) {
  switch (t) {
    case Quantity:
      current.quantity++;
      break;

    case Face:
      current.face++;
      break;
  }
  validate();
  return *this;
}

Bet& Bet::decrement(Type t) {
  switch (t) {
    case Quantity:
      current = last;
      current.quantity += 1;
      break;

    case Face:
      current = last;
      current.face += 1;
      break;
  }
  validate();
  return *this;
}

// should we call increment/decrement here?
void Bet::validate() {
  if (current.quantity < last.quantity)
    current.quantity = last.quantity;
  else if (current.quantity > game->getNumDice())
    current.quantity = game->getNumDice();
  else if (current.quantity <= 0)
    current.quantity = 1;

  if (current.face < last.face)
    current.face = last.face;
  else if (current.face > numFaces)
    current.face = numFaces;

  if (current.quantity == last.quantity && current.face == last.face) {
    if (current.quantity == game->getNumDice())
      current.face++;
    else
      current.quantity++;
  }
}