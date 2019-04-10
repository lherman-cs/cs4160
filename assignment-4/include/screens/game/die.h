#pragma once
#include "core/interface.h"
#include "global/imageFactory.h"
#include "sprite/sprite.h"
#include "util/vector2f.h"

class Die : public Drawable, public Updatable {
 public:
  enum State { visible, invisible, hidden };

  Die();
  Die(const Vector2f&, State, int val = 0);
  Die(const Die&) = delete;
  Die& operator=(const Die&) = delete;
  void draw() const;
  Die& select();
  Die& deselect();
  void update(Uint32 ticks);
  Die& set(const Vector2f& position);
  Die& set(const Die::State state);
  Die& set(int value);

  static int getSize();

 private:
  std::vector<Image*> faces{
      ImageFactory::getInstance().getImages("screens/game/dice")};
  Sprite hide{"screens/game/hidden-dice"};
  Sprite gone{"screens/game/invisible-dice"};
  State state = visible;
  int value = 0;
  Vector2f position = Vector2f(0, 0);
};