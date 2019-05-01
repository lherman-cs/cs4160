#include "widget/button.h"
#include "global/global.h"
#include "util/ioMod.h"

Button::Button() { button.setPosition(Vector2f(447, 465)); }

void Button::draw() const {
  if (showing) button.draw();
}

void Button::update(Uint32 ticks) { (void)ticks; }