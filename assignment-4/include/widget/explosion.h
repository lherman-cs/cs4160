#pragma once
#include "widget.h"

class WidgetController;

class Explosion : public Widget {
 public:
  virtual void draw() const;
  virtual void update(Uint32 ticks);

 private:
  friend class WidgetController;
  Explosion(Vector2f pos);
  MultiSprite explosion{"widget/explosion"};
};