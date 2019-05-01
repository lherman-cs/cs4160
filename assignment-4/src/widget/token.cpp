#include "widget/token.h"
#include "global/global.h"
#include "util/ioMod.h"

Token::Token() { token.setPosition(positions[0]); }

void Token::draw() const {
  if (showing) token.draw();
}

void Token::update(Uint32 ticks) { (void)ticks; }

void Token::set(const unsigned int& i) { token.setPosition(positions[i]); }