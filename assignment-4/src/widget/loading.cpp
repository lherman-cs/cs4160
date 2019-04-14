#include "widget/loading.h"
#include "util/ioMod.h"

Loading::Loading(std::string text) : text(text) {
  std::cout << "[loading] finished initializing" << std::endl;
  loading.setPosition(Vector2f(470, 300));
}

void Loading::draw() const {
  if (!showing) return;

  modal.draw();
  loading.draw();
  writer.writeText(text, 390, 430, loadingColor);
}

void Loading::update(Uint32 ticks) { loading.update(ticks); }