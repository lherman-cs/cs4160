#pragma once
#include "core/interface.h"
#include "global/imageFactory.h"
#include "sprite/sprite.h"
#include "util/vector2f.h"

class Die : public Drawable {
 public:
  enum State { visible, invisible, hidden };

  Die();
  Die(const Vector2f&, State, int val = 0);
  void draw() const;
  Die& select();
  Die& deselect();
  Die& set(const Vector2f& position);
  Die& set(const Die::State state);
  Die& set(int value);

  static int getSize();

 private:
  static std::vector<Image*> unhighlightedDice;
  static std::vector<Image*> highlightedDice;
  static Sprite hide;
  static Sprite gone;

  std::vector<Image*>* faces = &unhighlightedDice;
  State state = visible;
  int value = 0;
  Vector2f position = Vector2f(0, 0);
};

std::vector<Image*> Die::unhighlightedDice(
    ImageFactory::getInstance().getImages("screens/game/dice"));
std::vector<Image*> Die::highlightedDice(
    ImageFactory::getInstance().getImages("screens/game/highlighted-dice"));
Sprite Die::hide("screens/game/hidden-dice");
Sprite Die::gone("screens/game/invisible-dice");