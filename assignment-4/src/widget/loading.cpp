#include "widget/loading.h"
#include "util/ioMod.h"

Loading& Loading::getInstance() {
  static Loading instance;
  return instance;
}

Loading& Loading::create(std::string text) {
  this->text = text;
  return *this;
}

Loading::Loading() { loading.setPosition(Vector2f(470, 300)); }

void Loading::draw() const {
  if (!showing) return;

  modal.draw();
  loading.draw();
  writer.writeText(text, 390, 430, loadingColor);
}

void Loading::update(Uint32 ticks) { loading.update(ticks); }