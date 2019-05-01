#pragma once
#include "widget.h"

class WidgetController;

class Button : public Widget {
 public:
  Button();
  virtual void draw() const;
  virtual void update(Uint32 ticks);

 private:
  friend class WidgetController;
  Sprite button{"screens/game/call-button"};
};